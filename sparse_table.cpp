#include <bits/stdc++.h>

using namespace std;

#define sz(x) (int)(x.size())

#define nn cout << '\n'

#define forr(i,a,b) for(int i=int(a);i<int(b);++i)
#define forn(i,n) forr(i,0,n)

typedef long long ll;

using vi = vector<int>;
using vvi = vector<vi>;

#define oper min 
// Opciones que funciona: min, max, gcd, and, or. Cualquier op asociativa e idempotente
// Construcción: O(n log n) y query O(1)
// Query: [l, r)

struct SparseTable {
    int n, K;
    vvi st;

    SparseTable(const vi &v){
        n = sz(v); K = __lg(n) + 1;
        st.assign(K, vi(n));
        st[0] = v;
        forr(k, 1, K){
            forn(i, n - (1 << k) + 1){
                st[k][i] = oper(st[k - 1][i], st[k - 1][i + (1 << (k - 1))]);
            }
        }
    }

    int query(int l, int r){
        int k = __lg(r - l);
        return oper(st[k][l], st[k][r - (1 << k)]);
    }
};

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    vi v = {5, 8, 2, 7, 3, 9, 1, 6};

    SparseTable st(v);

    cout << st.query(0, 4); // min de [5, 8, 2, 7] = 2
    nn;

    cout << st.query(1, 5); // min de [8, 2, 7, 3] = 2
    nn;

    cout << st.query(3, 6); // min de [7, 3, 9] = 3
    nn;

    cout << st.query(4, 8); // min de [3, 9, 1, 6] = 1
    nn;

    cout << st.query(5, 6); // min de [9] = 9
    nn;

    return 0;
}