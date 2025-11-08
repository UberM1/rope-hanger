#ifndef ROPE_H
#define ROPE_H

#include "structures.h"
#include <vector>

template <typename Op>
requires Monoid<Op>
class Rope {
  private:
    std::vector<typename Op::Value> data;
    int n;
    typename Op::Value myQuyery(int l, int r, int node, int node_l, int node_r) {
      if (r <= node_l || node_r <= l) return Op::neut();
      if (l <= node_l && node_r <= r) return data[node];
      int mid = (node_l + node_r) / 2;
      return Op::op(
        myQuyery(l, r, 2*node+1, node_l, mid), myQuyery(l, r, 2*node+2, mid, node_r)
      );
    }

    void myUpdate(int l, int r, typename Op::Value x, int node, int node_l, int node_r) {
      if(r <= node_l || node_r <= l) return;
      if(l <= node_l && node_r <= r) { 
        data[node] = x;
        return;
      }
      int mid = (node_l + node_r) / 2;
      myUpdate(l, r, x, 2*node+1, node_l, mid);
      myUpdate(l, r, x, 2*node+2, mid, node_r);
      data[node] = Op::op(data[2*node+1], data[2*node+2]);
    }

  public:
    Rope(int n) {
      int tree_size = 1;
      while (tree_size < n) {
        tree_size *= 2;
      }
      data.resize(2 * tree_size - 1, Op::neut());
      this->n = tree_size;
    } // go for the smallest pow2 size 4 full tree
    
    typename Op::Value query(int l, int r) { 
      return myQuyery(l, r, 0, 0, this->n);
    }
    
    void update(int i, typename Op::Value x) { 
      myUpdate(i, i+1, x, 0, 0, this->n); 
    }

    std::vector<typename Op::Value> get_data() {
      return data; // not a direct data reference
    }
};

#endif