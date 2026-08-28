#include <bits/stdc++.h>

using namespace std;

#define pb push_back
#define all(x) x.begin(), x.end()

#define nn cout << '\n'

#define forr(i,a,b) for(int i = int(a); i < int(b); ++i)
#define forn(i,n) forr(i,0,n)

typedef long long ll;

using vi = vector<int>;
using vl = vector<ll>;

const ll INF = 1e18 + 100;

// BELLMAN-FORD
// Caminos minimos desde s, permite pesos negativos
// Complejidad: O(n * m)
// Detecta ciclos negativos alcanzables desde s
//
// bellman_ford(s):
//   true  -> no hay ciclo negativo alcanzable
//   false -> hay ciclo negativo
//
// dist[v] = distancia desde s
// parent[v] = padre de v para la reconstruccion

struct Edge{
    int u, v; ll w;
};

int n, m;
vector<Edge> edges;

vl dist;
vi parent;
int neg_cycle;

// Se puede hacer que el bellman-ford detecte la presencia de algún ciclo negativo. Cambios indicados
// La lógica es que se agrega un nodo que se conecta a todos con peso 0
bool bellman_ford(int s){   // Se borra el parametro
    dist.assign(n, INF);    // dist = 0
    parent.assign(n, -1);
    dist[s] = 0;            // borrar
    neg_cycle = -1;
    forn(i, n){
        neg_cycle = -1;
        for(auto [u, v, w] : edges){
            if(dist[u] == INF) continue;
            if(dist[v] > dist[u] + w){
                dist[v] = max(-INF, dist[u] + w);
                parent[v] = u;
                neg_cycle = v;
            }
        }
    }
    return neg_cycle == -1;
}

vi get_negative_cycle(){ // Devuelve algún ciclo negativo
    if(neg_cycle == -1) return {};
    int v = neg_cycle;
    forn(i, n) v = parent[v];
    vi cycle;
    int u = v;
    do {
        cycle.pb(u);
        u = parent[u];
    } while(u != v);
    cycle.pb(v);
    reverse(all(cycle));
    return cycle;
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    cin >> n >> m;
    edges.resize(m);
    for(auto &[u, v, w] : edges){
        cin >> u >> v >> w; --u; --v;
    }
    if(bellman_ford(0)){
        cout << "No hay ciclo negativo"; nn;
        // Distancias
        forn(i, n){
            if(dist[i] == INF)
                cout << "INF";
            else
                cout << dist[i];
            nn;
        }
    } else {
        cout << "Ciclo negativo:"; nn;
        vi cycle = get_negative_cycle();
        for(int v : cycle) cout << v << " ";
        nn;
    }
}