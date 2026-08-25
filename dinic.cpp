#include <bits/stdc++.h>

using namespace std;

#define pb push_back
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x.size())

#define nn cout << '\n'

#define forr(i,a,b) for(int i = int(a); i < int(b); ++i)
#define forn(i,n) forr(i,0,n)

typedef long long ll;

using vi = vector<int>;

const ll INF = 1e18 + 100;

// DINIC - Maximum Flow
// Complejidad general: O(V^2 * E)
// add_edge(u, v, cap): agrega arista dirigida u -> v con capacidad cap
// max_flow(s, t): devuelve el flujo maximo entre s y t
//
// Despues de max_flow:
// dist[v] >= 0 <=> v queda del lado de la fuente en un min-cut
// dist[v] < 0  <=> v queda del lado del sink
//
// Para matching bipartito con capacidades 1:
// source -> izquierda -> derecha -> sink

struct Dinic {
    struct Edge { 
        int to, rev;
        ll flow, cap;
    };
    int n, src, dst;
    vi dist, q, work;
    vector<vector<Edge>> g;

    Dinic(int n): n(n), dist(n), q(n), work(n), g(n) {}

    void add_edge(int u, int v, ll cap){
        g[u].pb({v, sz(g[v]), 0, cap});
        g[v].pb({u, sz(g[u]) - 1, 0, 0});
    }

    bool bfs(){
        fill(all(dist), -1);
        dist[src] = 0;
        int qh = 0, qt = 0;
        q[qt++] = src;
        while(qh < qt){
            int u = q[qh++];
            for(Edge &e : g[u]){
                if(dist[e.to] == -1 && e.flow < e.cap){
                    dist[e.to] = dist[u] + 1;
                    q[qt++] = e.to;
                }
            }
        }
        return dist[dst] != -1;
    }

    ll dfs(int u, ll pushed){
        if(u == dst) return pushed;
        for(int &i = work[u]; i < sz(g[u]); i++){
            Edge &e = g[u][i];
            if(e.flow == e.cap) continue;
            if(dist[e.to] != dist[u] + 1) continue;
            ll delta = dfs(e.to, min(pushed, e.cap - e.flow));
            if(delta){
                e.flow += delta;
                g[e.to][e.rev].flow -= delta;
                return delta;
            }
        }
        return 0;
    }

    ll max_flow(int s, int t){
        src = s; dst = t;
        ll ans = 0;
        while(bfs()){
            fill(all(work), 0);
            while(ll delta = dfs(src, INF)) ans += delta;
        }
        return ans;
    }
};

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    Dinic dinic(6);

    dinic.add_edge(0, 1, 10);
    dinic.add_edge(0, 2, 10);
    dinic.add_edge(1, 2, 2);
    dinic.add_edge(1, 3, 4);
    dinic.add_edge(1, 4, 8);
    dinic.add_edge(2, 4, 9);
    dinic.add_edge(4, 3, 6);
    dinic.add_edge(3, 5, 10);
    dinic.add_edge(4, 5, 10);

    cout << "Max flow: " << dinic.max_flow(0, 5); nn;

    cout << "Lado source del min-cut: ";
    forn(i, 6){
        if(dinic.dist[i] >= 0) cout << i << " ";
    }
    nn;

    cout << "Lado sink del min-cut: ";
    forn(i, 6){
        if(dinic.dist[i] < 0) cout << i << " ";
    }
    nn;

    return 0;
}