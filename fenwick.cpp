#include <bits/stdc++.h>
using namespace std;

#define forr(i,a,b) for(int i=int(a);i<int(b);++i)
#define forn(i,n) forr(i,0,n)

typedef long long ll;

// Fenwick Tree - indices externos 0-based
// upd(i, v):      a[i] += v               O(log n)
// get(i):         suma de [0, i)          O(log n)
// get_range(l,r): suma de [l, r)          O(log n)
//
// Para usar XOR cambiar:
//      +=  por ^=     en upd
//      +=  por ^=     en get
//      -   por ^      en get_range

struct Fenwick {
    int n;
    vector<ll> ft;

    Fenwick(int n): n(n), ft(n + 1, 0) {}

    void upd(int i, ll v){
        for(i++; i <= n; i += i & -i) ft[i] += v;
        // XOR: ft[i] ^= v;
    }

    ll get(int i){
        ll res = 0;
        for(; i > 0; i -= i & -i) res += ft[i];
        // XOR: res ^= ft[i];
        return res;
    }

    ll get_range(int l, int r){
        return get(r) - get(l);
        // XOR: get(r) ^ get(l);        
    }
};


int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n;
    cin >> n;

    vector<ll> a(n);

    forn(i, n) cin >> a[i];
    Fenwick ft(n);

    // Cargar vector inicial: O(n log n)
    forn(i, n) ft.upd(i, a[i]);

    // suma [0, 4)
    cout << ft.get(4) << '\n';

    // suma [2, 5)
    cout << ft.get_range(2, 5) << '\n';

    // a[3] += 10
    ft.upd(3, 10);
}