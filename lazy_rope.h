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
 public:
  LazyRope(int n) { /* COMPLETAR */ }
  typename Op::Value query(int l, int r) { /* COMPLETAR */ }
  void update(int l, int r, typename Op::Update x) { /* COMPLETAR */ }

 private:
  std::vector<typename Op::Value> data;
  std::vector<typename Op::Update> lazy;
};

#endif // LAZY_ROPE_H