#include "structures.h"
#include "rope.h"
#include "lazy_rope.h"
#include <cassert>
#include <iostream>
#include <set>
#include <algorithm>
#include <iterator> 

struct IntSum {
    using Value = int;
    using Update = int;
    static int op(int a, int b) { return a + b; }
    static int neut() { return 0; }
};

static_assert(Monoid<IntSum>, "IntSum must satisfy Monoid concept");

struct SetUnion {
    using Value = std::set<int>;
    using Update = std::set<int>;
    static Value op(const Value &a, const Value &b) {
        Value res;
        // std::set_union(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(res));
        std::set_union(a.begin(), a.end(), b.begin(), b.end(), std::inserter(res, res.end()));
        return res;
    }
    static Value neut() { return {}; }
};

static_assert(Monoid<SetUnion>, "SetUnion must satisfy Monoid concept");

struct StringConcat {
    using Value = std::string;
    static Value op(const Value &a, const Value &b) { return a + b; }
    static Value neut() { return ""; }
};

static_assert(Monoid<StringConcat>, "StringConcat must satisfy Monoid concept");

template<typename Monoid>
void print_rope_tree(Rope<Monoid> &rope, int l, int r, const std::string &prefix = "", bool isRight = true) {
    if (l >= r) return;
    auto sum = rope.query(l, r);
    std::cout << prefix;
    std::cout << (isRight ? "┌─ " : "└─ ");
    std::cout << "[" << l << "," << r << ") sum=" << sum << "\n";
    if (r - l <= 1) return; // hoja
    int mid = (l + r) / 2;
    print_rope_tree(rope, mid, r, prefix + (isRight ? "│  " : "   "), true);
    print_rope_tree(rope, l, mid, prefix + (isRight ? "│  " : "   "), false);
}

int tests_IntSum() {
    Rope<IntSum> intRope(4);
    assert(intRope.query(0,4) == IntSum::neut());
    assert(intRope.query(0,1) == IntSum::neut());
    assert(intRope.query(2,3) == IntSum::neut());

    intRope.update(0, 1);
    intRope.update(1, 2);
    intRope.update(2, 3);
    intRope.update(3, 1);
    
    std::vector<int> data = intRope.get_data();
    assert((data == std::vector<int>{7,3,4,1,2,3,1}));

    int result = intRope.query(0,4);
    assert(result == 7);
    
    intRope.update(2, 5);
    data = intRope.get_data();
    assert((data == std::vector<int>{9,3,6,1,2,5,1}));

    result = intRope.query(0,4);
    assert(result == 9);

    std::cout << "IntSum tests passed!!\n";

    print_rope_tree(intRope, 0, 4);

    return 0;
}

int tests_SetUnion() {
    Rope<SetUnion> setRope(3);
    std::cout << "before update!!\n";

    setRope.update(0, {1, 2});
    setRope.update(1, {2, 3});
    setRope.update(2, {4});
    auto result = setRope.query(0, 3);
    assert((result == std::set<int>({1, 2, 3, 4})));
    std::cout << "SetUnion tests passed!!\n";
    return 0;
}

int tests_StringConcat() {
    Rope<StringConcat> stringRope(3);
    stringRope.update(0, "Hello ");
    stringRope.update(1, "World");
    stringRope.update(2, "!");
    auto result = stringRope.query(0, 3);
    assert((result == "Hello World!"));
    std::cout << "StringConcat tests passed!!\n";
    return 0;
}

int tests_LazyIntSum() {
    LazyRope<IntSum> lazyRope(4);
    
    // Test initial state
    assert(lazyRope.query(0,4) == 0);
    auto lazy_data = lazyRope.get_lazy_data();
    assert((lazy_data == std::vector<int>(7, 0)));
    
    // Range update [0,4) with value 5
    lazyRope.update(0, 4, 5);
    assert(lazyRope.query(0,1) == 5);
    assert(lazyRope.query(1,2) == 5);
    assert(lazyRope.query(0,4) == 20); // 5*4 = 20
    
    // Partial range update [1,3) with value 3
    lazyRope.update(1, 3, 3);
    assert(lazyRope.query(0,1) == 5);
    assert(lazyRope.query(1,2) == 8); // 5+3
    assert(lazyRope.query(2,3) == 8); // 5+3
    assert(lazyRope.query(3,4) == 5);
    assert(lazyRope.query(0,4) == 26); // 5+8+8+5
    
    // Test lazy_data state after updates
    lazy_data = lazyRope.get_lazy_data();
    // Verify that lazy propagation is working (some values should be non-zero)
    bool has_lazy_values = false;
    for(int val : lazy_data) {
        if(val != 0) has_lazy_values = true;
    }
    assert(has_lazy_values);

    std::cout << "LazyIntSum tests passed!!\n";
    return 0;
}

int tests_LazySetUnion() {
    LazyRope<SetUnion> lazyRope(3);
    
    // Test initial state
    assert(lazyRope.query(0,3).empty());
    auto lazy_data = lazyRope.get_lazy_data();
    assert((lazy_data == std::vector<std::set<int>>(5, std::set<int>{})));
    
    // Range update [0,3) with set {1,2}
    lazyRope.update(0, 3, {1, 2});
    assert((lazyRope.query(0,1) == std::set<int>({1, 2})));
    assert((lazyRope.query(1,2) == std::set<int>({1, 2})));
    assert((lazyRope.query(0,3) == std::set<int>({1, 2})));
    
    // Partial range update [1,2) with set {3,4}
    lazyRope.update(1, 2, {3, 4});
    assert((lazyRope.query(0,1) == std::set<int>({1, 2})));
    assert((lazyRope.query(1,2) == std::set<int>({1, 2, 3, 4})));
    assert((lazyRope.query(2,3) == std::set<int>({1, 2})));
    assert((lazyRope.query(0,3) == std::set<int>({1, 2, 3, 4})));
    
    std::cout << "LazySetUnion tests passed!!\n";
    return 0;
}

int main() {
    tests_IntSum();
    tests_SetUnion();
    tests_StringConcat();
    tests_LazyIntSum();
    tests_LazySetUnion();
    return 0;
}
