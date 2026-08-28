#include <bits/stdc++.h>

using namespace std;

#define pb push_back
#define sz(x) (int)(x.size())

#define nn cout << '\n'

#define forr(i,a,b) for(int i = int(a); i < int(b); ++i)
#define forn(i,n) forr(i,0,n)

typedef long long ll;

using pll = pair<ll, ll>;
using vl = vector<ll>;
using vll = vector<pll>;

const ll INF = 1e18 + 100;

// DIJKSTRA
// Complejidad: O((n + m) log n)
// g[u] = {peso, destino}
// dijkstra(s, g) devuelve las distancias desde s

vl dijkstra(int s, vector<vll> &g){
    int n = sz(g);
    vl dist(n, INF);
    priority_queue<pll, vll, greater<pll>> pq;
    dist[s] = 0;
    pq.push({0, s});
    while(!pq.empty()){
        auto [d, u] = pq.top();
        pq.pop();
        if(d != dist[u]) continue;
        for(auto [w, v] : g[u]){
            if(dist[v] > d + w){
                dist[v] = d + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}


int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n = 5;
    vector<vll> g(n);

    // Grafo no dirigido
    auto add_edge = [&](int u, int v, ll w){
        g[u].pb({w, v});
        g[v].pb({w, u});
    };

    add_edge(0, 1, 4);
    add_edge(0, 2, 1);
    add_edge(2, 1, 2);
    add_edge(1, 3, 1);
    add_edge(2, 3, 5);
    add_edge(3, 4, 3);

    vl dist = dijkstra(0, g);

    forn(i, n){
        cout << "dist(0, " << i << ") = " << dist[i];
        nn;
    }

    return 0;
}