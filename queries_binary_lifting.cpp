#include <bits/stdc++.h>

using namespace std;

#define forr(i,a,b) for(int i=int(a);i<int(b);++i)
#define forn(i,n) forr(i,0,n)
#define dforr(i,a,b) for(int i = int(b)-1;i>=int(a);i--)
#define dforn(i,n) dforr(i,0,n)
using vi = vector<int>;
using vvi = vector<vi>;

const int LOG = 20;

int main(){
    // Construccion del binary lifting
    int n;
    vi nxt(n); 
    // nxt[i] indica el siguiente nodo/posicion.
    // Valores entre 0 y n, donde n es un nodo centinela.
    // Debe cumplirse nxt[i] > i para i < n,
    // y conceptualmente nxt[n] = n.
    vvi up(n + 1, vector<int>(LOG, n));
    forn(i, n) up[i][0] = nxt[i];
    forr(j, 1, LOG){
        forn(i, n) up[i][j] = up[up[i][j-1]][j-1];
    }

    // Queries
    int q;
    while(q--){
        int l, r;
        int ans = 0; // Cuenta saltos. Si es valores empieza en 1
        dforn(j, LOG){
            if(up[l][j] <= r){
                l = up[l][j];
                ans += (1 << j);
            }
        }
        cout << ans << endl;
    }

    return 0;
}