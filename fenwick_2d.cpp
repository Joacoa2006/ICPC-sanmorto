#include <bits/stdc++.h>
using namespace std;

#define forr(i,a,b) for(int i=int(a);i<int(b);++i)
#define forn(i,n) forr(i,0,n)

typedef long long ll;

// Fenwick Tree 2D - indices externos 0-based - Operaciones en O(log n log m)
// upd(r, c, v): a[r][c] += v
// get(r, c): suma de filas [0,r) y columnas [0,c)
// get_range(r1,c1,r2,c2): suma del rectangulo - filas [r1,r2) - columnas [c1,c2)

struct Fenwick2D {
    int n, m;
    vector<vector<ll>> ft;
    Fenwick2D(int n, int m): n(n), m(m), ft(n + 1, vector<ll>(m + 1, 0)) {}
    void upd(int r, int c, ll v){
        for(int i = r + 1; i <= n; i += i & -i){
            for(int j = c + 1; j <= m; j += j & -j){
                ft[i][j] += v;
            }
        }
    }
    ll get(int r, int c){
        ll res = 0;
        for(int i = r; i > 0; i -= i & -i){
            for(int j = c; j > 0; j -= j & -j){
                res += ft[i][j];
            }
        }
        return res;
    }
    ll get_range(int r1, int c1, int r2, int c2){
        return get(r2, c2) - get(r1, c2) - get(r2, c1) + get(r1, c1);
    }
};



int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n = 4, m = 5;
    vector<vector<ll>> a = {
        {1, 2, 3, 4, 5},
        {6, 7, 8, 9, 10},
        {11, 12, 13, 14, 15},
        {16, 17, 18, 19, 20}
    };
    Fenwick2D ft(n, m);
    // Cargar matriz inicial: O(n*m*log n*log m)
    forn(i, n){
        forn(j, m){
            ft.upd(i, j, a[i][j]);
        }
    }
    cout << ft.get(2, 3) << '\n'; // 27
    cout << ft.get_range(1, 2, 3, 5) << '\n'; // 69
    ft.upd(2, 3, 100);
    cout << ft.get_range(1, 2, 3, 5) << '\n'; // 169
}