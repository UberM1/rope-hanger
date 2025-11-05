#ifndef LAZY_ROPE_H
#define LAZY_ROPE_H

#include "structures.h"
#include <vector>

template <typename T>
concept LazyUpdate = requires {
  typename T::Value;
  typename T::Update;
} && requires(typename T::Update up, typename T::Value val, int len) {
  { T::apply(up, val, len) } -> std::same_as<typename T::Value>;
};

template <typename Op>
  requires LazyUpdate<Op>
class LazyRope {
private:
  void lazy_update(int l, int r, typename Op::Update x, int node, int node_l, int node_r) {
    // Implementación de lazy_update
  }

  // lazy_query:


 public:
  LazyRope(int n) {
    int tree_size = 1;
    while (tree_size < n) {
      tree_size <<= 1;
    }
    data.resize(2 * tree_size - 1, Op::neut());
    lazy_data.resize(2 * tree_size - 1, Op::neut());
    this->n = tree_size;
  }

  typename Op::Value query(int l, int r) {
    return lazy_query(l, r, 0, 0, n);
  }

  void update(int l, int r, typename Op::Update x) {
    lazy_update(l, r, x, 0, 0, n);
  }

  void lazy_update(int l, int r, typename Op::Update x) {
    local_lazy_update(l, r, x, 0, 0, n);
  }

 private:
  std::vector<typename Op::Value> data;
  std::vector<typename Op::Update> lazy_data;
};

#endif // LAZY_ROPE_H