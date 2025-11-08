#ifndef LAZY_ROPE_H
#define LAZY_ROPE_H

#include <vector>

#include "structures.h"

template <typename Op>
requires Monoid<Op>
class LazyRope {
 private:
  int n;
  std::vector<typename Op::Value> data;
  std::vector<typename Op::Update> lazy_data;

  int izq(int n) { return 2 * n + 1; }
  int der(int n) { return 2 * n + 2; }

  void apply(int pos, int times) {
    while (times > 0) {
      data[pos] = Op::op(data[pos], lazy_data[pos]);
      times -= 1;
    }
  }

  void propagate(int node, int l_, int r_) {
    int len = r_ - l_;
    if (len > 1) {  // no es hoja
      lazy_data[izq(node)] = Op::op(lazy_data[izq(node)], lazy_data[node]);
      lazy_data[der(node)] = Op::op(lazy_data[der(node)], lazy_data[node]);
    }

    apply(node, len);
    lazy_data[node] = Op::neut();
  }

  void lazy_update(int node, int l_, int r_, int l, int r, typename Op::Update upd) {
    if (r <= l_ || r_ <= l) {
      return;
    }
    if (l <= l_ && r_ <= r) {
      lazy_data[node] = Op::op(lazy_data[node], upd);
      return;
    }
    int m_ = (l_ + r_) / 2;
    lazy_update(izq(node), l_, m_, l, r, upd);
    lazy_update(der(node), m_, r_, l, r, upd);
    data[node] = Op::op(data[izq(node)], data[der(node)]);
  }
 

  typename Op::Value lazy_query(int l, int r, int node, int l_, int r_) {
    if (r <= l_ || r_ <= l) {
      return Op::neut();
    }
    
    if (lazy_data[node] != Op::neut()) {
      propagate(node, l_, r_);
    }
    
    if (l <= l_ && r_ <= r) {
      return data[node];
    }
    int m_ = (l_ + r_) / 2;
    return Op::op(lazy_query(l, r, izq(node), l_, m_),
                  lazy_query(l, r, der(node), m_, r_));
  }

 public:
  LazyRope(int n) {
    int tree_size = 1;
    while (tree_size < n) {
      tree_size *= 2;
    }
    data.resize(2 * tree_size - 1, Op::neut());
    lazy_data.resize(2 * tree_size - 1, Op::neut());
    this->n = tree_size;
  }

  typename Op::Value query(int l, int r) { return lazy_query(l, r, 0, 0, n); }

  void update(int l, int r, typename Op::Update x) {
    lazy_update(0, 0, n, l, r, x);
  }

  std::vector<typename Op::Value> get_data() {
    return data;  // not a direct data reference
  }

  std::vector<typename Op::Update> get_lazy_data() {
    return lazy_data;  // not a direct data reference
  }
};

#endif  // LAZY_ROPE_H