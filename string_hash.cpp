#include <bits/stdc++.h>

using namespace std;

#define all(x) x.begin(), x.end()
#define sz(x) (int)(x.size())

typedef long long ll;

const ll P = 31;
const ll MOD = 999727999;

// ------------------------------------------------------------
// STRING HASH
//
// get(l,r) -> hash de s[l,r) en O(1)
//
// Usos:
// - comparar substrings
// - buscar repeticiones / periodicidad
// - LCP de dos suffixes con binary search
// - palindromos usando hash de reverse(s)
//
// Construccion: O(n)
// get: O(1)
// lcp: O(log n)
// ------------------------------------------------------------

struct Hash {
    vector<ll> h, pw;
    Hash(const string &s){
        int n = sz(s);
        h.assign(n+1, 0);
        pw.assign(n+1, 1);
        for(int i=0; i<n; i++){
            pw[i+1] = pw[i] * P % MOD;
            h[i+1] = (h[i] * P + (s[i]-'a'+1)) % MOD;
        }
    }
    ll get(int l, int r){ // s[l,r)
        return (h[r] - h[l]*pw[r-l] % MOD + MOD) % MOD;
    }
};

// LCP de los suffixes que empiezan en i y j
int lcp(int i, int j, Hash &h, int n){
    int l = 0;
    int r = min(n-i, n-j) + 1;
    while(r-l > 1){
        int m = (l+r)/2;
        if(h.get(i, i+m) == h.get(j, j+m)) l = m;
        else r = m;
    }
    return l;
}

// Hash h de s
// Hash rh de reverse(s)
// Determina si s[l,r) es palindromo
bool is_pal(int l, int r, Hash &h, Hash &rh, int n){
    return h.get(l,r) == rh.get(n-r, n-l);
}

int main(){
    string s;
    cin >> s;
    int n = sz(s);
    Hash h(s);
    string rev = s;
    reverse(all(rev));
    Hash rh(rev);

    // Hash de substring [l,r)
    int l = 1, r = 4;
    cout << h.get(l,r) << '\n';

    // Comparar dos substrings
    int l1 = 0, r1 = 3;
    int l2 = 3, r2 = 6;

    if(h.get(l1,r1) == h.get(l2,r2)) cout << "Iguales\n";

    // LCP de suffixes
    cout << lcp(0, 3, h, n) << '\n';

    // Palindromo
    if(is_pal(l, r, h, rh, n)) cout << "Palindromo\n";
}