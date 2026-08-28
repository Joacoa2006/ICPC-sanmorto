#include <bits/stdc++.h>

using namespace std;

#define pb push_back
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x.size())

#define nn cout << '\n'

#define forr(i,a,b) for(int i = int(a); i < int(b); ++i)
#define forn(i,n) forr(i,0,n)

typedef long long ll;

const int MAXN = 505;
const ll INF = 1e18 + 100;

// FLOYD-WARSHALL
// Caminos minimos entre todos los pares
// Permite pesos negativos
// Complejidad: O(n^3)
// Memoria: O(n^2)
//
// dist[i][j] = distancia minima de i a j
//
// inNegCycle(v):
//   true si v pertenece a algun ciclo negativo.
//
// hasNegCycle(a, b):
//   true si existe un ciclo negativo alcanzable desde a
//   y desde el cual se puede llegar a b.
//   En ese caso la distancia minima a -> b es -infinito.

int n, m;
ll dist[MAXN][MAXN];
void floyd_warshall(){
    forn(k, n){
        forn(i, n){
            forn(j, n){
                if(dist[i][k] == INF || dist[k][j] == INF) continue;
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }
}
bool inNegCycle(int v){
    return dist[v][v] < 0;
}
bool hasNegCycle(int a, int b){
    forn(i, n){
        if(dist[a][i] < INF && dist[i][b] < INF && dist[i][i] < 0) return true;
    }
    return false;
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    cin >> n >> m;
    // Inicializacion
    forn(i, n){
        forn(j, n) dist[i][j] = INF;
        dist[i][i] = 0;
    }
    // Grafo dirigido
    forn(i, m){
        int u, v;
        ll w;
        cin >> u >> v >> w; --u; --v;
        dist[u][v] = min(dist[u][v], w);
    }
    floyd_warshall();
    return 0;
}