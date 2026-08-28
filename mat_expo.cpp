#include <bits/stdc++.h>

using namespace std;

#define sz(x) (int)(x.size())

#define nn cout << '\n'

#define forr(i,a,b) for(int i = int(a); i < int(b); ++i)
#define forn(i,n) forr(i,0,n)

typedef long long ll;

using vl = vector<ll>;
using vvl = vector<vl>;

const ll MOD = 1e9 + 7;

// MATRIX EXPONENTIATION
// A^e modulo MOD
// Complejidad: O(n^3 log e)

using Matrix = vvl;

Matrix mul(const Matrix &a, const Matrix &b){
    int n = sz(a);
    Matrix c(n, vl(n, 0));
    forn(i, n){
        forn(k, n){
            forn(j, n){
                c[i][j] = (c[i][j] + a[i][k] * b[k][j]) % MOD;
            }
        }
    }
    return c;
}

Matrix mat_pow(Matrix a, ll e){
    int n = sz(a);
    Matrix res(n, vl(n, 0));
    forn(i, n) res[i][i] = 1;
    while(e){
        if(e & 1) res = mul(res, a);
        a = mul(a, a);
        e >>= 1;
    }
    return res;
}


int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    Matrix fib = {
        {1, 1},
        {1, 0}
    };
    ll n = 10;
    Matrix r = mat_pow(fib, n);
    cout << r[0][1]; nn; // F_10 = 55
}