#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

#define all(x) x.begin(), x.end()
#define sz(x) (int)(x.size())

// Build: O(n log n). Query: O(log^2 n)
// Queries [a, b) indexadas desde 0
struct MSTree {
    vector<vector<int>> st; int n;
    MSTree(int n): st(4*n+5), n(n) {}
    void init(int k, int s, int e, int *a){
        if(s+1 == e){ st[k] = {a[s]}; return; }
        int m = (s+e)/2;
        init(2*k, s, m, a);
        init(2*k+1, m, e, a);
        st[k].reserve(sz(st[2*k]) + sz(st[2*k+1]));
        merge(all(st[2*k]), all(st[2*k+1]), back_inserter(st[k]));
    }
    // Modificar la estructura de la query para otras operaciones
    // Actualmente: cantidad de elementos <= a x en [a, b)
    int query(int k, int s, int e, int a, int b, int x){
        if(s >= b || e <= a) return 0;
        if(s >= a && e <= b) return upper_bound(all(st[k]), x) - st[k].begin();
        int m = (s+e)/2;
        return query(2*k, s, m, a, b, x) + query(2*k+1, m, e, a, b, x);
    }
    void init(int *a){ init(1, 0, n, a); }
    int query(int a, int b, int x){ return query(1, 0, n, a, b, x); }
}; // usage: MSTree mst(n); mst.init(x);

int main(){
    vector<int> v = {7, 2, 5, 1, 8, 3};
    MSTree mst(sz(v));
    mst.init(&v[0]);
    cout << mst.query(1, 5, 5);
    return 0;
}