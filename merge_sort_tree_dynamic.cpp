#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

using ordered_set = tree<pair<int,int>, null_type, less<pair<int,int>>, rb_tree_tag, tree_order_statistics_node_update>;

// Build O(n log^2 n). Query y update O(log^2 n)
struct MSTree {
    vector<ordered_set> st;
    vector<int> a;
    int n;
    MSTree(int n): st(4*n+5), a(n), n(n) {}
    // Inserta inicialmente a[p] = v en todos los nodos que contienen p
    void insert_initial(int k, int s, int e, int p, int v){
        st[k].insert({v, p});
        if(s+1 == e) return;
        int m = (s+e)/2;
        if(p < m) insert_initial(2*k, s, m, p, v);
        else insert_initial(2*k+1, m, e, p, v);
    }
    void init(int *v){
        for(int i = 0; i < n; i++){
            a[i] = v[i]; insert_initial(1, 0, n, i, v[i]);
        }
    }
    // a[p] = v
    void upd(int k, int s, int e, int p, int old_v, int new_v){
        st[k].erase({old_v, p});
        st[k].insert({new_v, p});
        if(s+1 == e) return;
        int m = (s+e)/2;
        if(p < m) upd(2*k, s, m, p, old_v, new_v);
        else upd(2*k+1, m, e, p, old_v, new_v);
    }
    void upd(int p, int v){
        upd(1, 0, n, p, a[p], v);
        a[p] = v;
    }
    // Cantidad de elementos <= x en [a,b)
    int query(int k, int s, int e, int a, int b, int x){
        if(s >= b || e <= a) return 0;
        if(s >= a && e <= b){
            return st[k].order_of_key({x, INT_MAX});
        }
        int m = (s+e)/2;
        return query(2*k, s, m, a, b, x) + query(2*k+1, m, e, a, b, x);
    }
    int query(int a, int b, int x){ return query(1, 0, n, a, b, x); }
}; // usage: MSTree mst(n); mst.init(x); mst.query(a, b, x); mst.upd(p, v)

int main(){
    vector<int> v = {7, 2, 5, 1, 8, 3};
    MSTree mst(6);
    mst.init(&v[0]);
    cout << mst.query(1, 5, 5) << '\n';
    // cantidad de valores <= 5 en [1,5)
    mst.upd(2, 10);
    // x[2] = 10
    cout << mst.query(1, 5, 5) << '\n';
}