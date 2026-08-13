#include <bits/stdc++.h>

#define forr(i,a,b) for(int i=int(a);i<int(b);++i)
#define forn(i,n) forr(i,0,n)
#define dforr(i,a,b) for(int i = int(b)-1;i>=int(a);i--)
#define dforn(i,n) dforr(i,0,n)

using namespace std;

typedef long long ll;

const ll MOD = 1e9 + 7;

ll fast_pow(ll a, ll e, ll mod = MOD){
    ll res = 1;
    while(e){
        if(e & 1) res = res * a % mod;

        a = a * a % mod;
        e >>= 1;
    }
    return res;
} // O(log e)

// Requiere mod primo y 0 < a < mod
ll inv(ll a, ll mod = MOD){
    return fast_pow(a, mod - 2, mod);
} // O(log MOD)

int main(){
    int n, k;
    // Combinatorio
    vector<ll> fact(n + 1, 1);
    vector<ll> inv_fact(n + 1, 1);
    forr(i, 1, n + 1){
        fact[i] = fact[i - 1] * i % MOD;
        inv_fact[i] = inv(fact[i], MOD);
    }

    // Posible optimización para los inversos que reduce la complejidad a O(n + log MOD) en vez de O(n * log MOD)
    /*
    inv_fact[n] = inv(fact[n], MOD);

    dforr(i, 1, n + 1) inv_fact[i - 1] = inv_fact[i] * i % mod;
    */

    ll combinatorio = fact[n] * inv_fact[k] % MOD * inv_fact[n - k] % MOD;
}
