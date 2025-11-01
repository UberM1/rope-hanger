#include <concepts>
#include <vector>

template<typename T>
concept Monoid = requires(T::Value a, T::Value b, T::Value c) {
  typename T::Value; // hay un tipo de valores
  { T::op(a, b) } -> std::same_as<typename T::Value>; // clausura de la operacion
  // T::op(a, T::op(b, c)) == T::op(T::op(a, b), c) // asociatividad de la operacion
  { T::neut() } -> std::same_as<typename T::Value>; // existencia del neutro
  // T::op(a, T::neut()) == a // neutro por derecha
  // T::op(T::neut(), a) == a // neutro por izquierda
};

template <typename Op>
requires Monoid<Op>
class Rope {
  private:
    std::vector<typename Op::Value> data;
    typename Op::Value myQuyery(int l, int r, int node, int node_l, int node_r) {
      if (r <= node_l || node_r <= l) return Op::neut();
      if (l <= node_l && node_r <= r) return data[node];
      int mid = (node_l + node_r) / 2;
      return Op::op(
        myQuyery(l, r, 2*node+1, node_l, mid), myQuyery(l, r, 2*node+2, mid, node_r)
      );
    }

    typename Op::Value myUpdate(int l, int r, typename Op::Value x, int node, int node_l, int node_r) {
      if(r <= node_l || node_r <= l) return data[node];
      if(l <= node_l && node_r <= r) { data[node] = x; } // estoy en el elemento a actualizar
      int mid = (node_l + node_r) / 2;
      data[node] = Op::op(
        myUpdate(l, r, x, 2*node+1, node_l, mid), myUpdate(l, r, x, 2*node+2, mid, node_r)
      );
    }

  public:
    Rope(int n) {
      data.resize(2*n-1, Op::neut());
      typename Op::Value query(int l, int r) { return myQuyery(l, r, 0, 0, (data.size()+1)/2); }
      void update(int i, typename Op::Value x) { return myUpdate(i, i+1, x, 0, 0, (data.size()+1)/2); }
  }
};

// myupdate con una sola tirada
// typename Op::Value myUpdate(int l, int r, typename Op::Value x, int node, int node_l, int node_r) {
//       if (r <= node_l || node_r <= l) {
//           return data[node];
//       }
      
//       if (node_r - node_l == 1) {
//           data[node] = x;
//           return data[node];
//       }
      
//       int mid = (node_l + node_r) / 2;
//       data[node] = Op::op(
//           myUpdate(l, r, x, 2*node+1, node_l, mid),
//           myUpdate(l, r, x, 2*node+2, mid, node_r)
//       );
//       return data[node];
//   }


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
  Rope(int n) { /* COMPLETAR */ }
  typename Op::Value query(int l, int r) { /* COMPLETAR */ }
  void update(int l, int r, typename Op::Update x) { /* COMPLETAR */ }

 private:
  std::vector<typename Op::Value> data;
  std::vector<typename Op::Update> lazy;
};