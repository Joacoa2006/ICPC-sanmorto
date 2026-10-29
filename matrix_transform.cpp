#include <bits/stdc++.h>

using namespace std;

#define all(x) x.begin(), x.end()
#define sz(x) (int)(x.size())
#define forr(i,a,b) for(int i=int(a);i<int(b);++i)
#define forn(i,n) forr(i,0,n)

using vi = vector<int>;
using vvi = vector<vi>;

vvi rotate90(const vvi& a){
    int n = sz(a), m = sz(a[0]);
    vvi b(m, vi(n));
    forn(i,n) forn(j,m) b[j][n-1-i] = a[i][j];
    return b;
}

void flipH(vvi & a){ // Espejar horizontal
    reverse(all(a));
}

void flipV(vvi & a){ // Espejar vertical
    for(auto & r : a) reverse(all(r));
}

