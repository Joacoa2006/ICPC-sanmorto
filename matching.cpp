#include <bits/stdc++.h>

using namespace std;

#define pb push_back

#define all(x) x.begin(), x.end()
#define sz(x) (int)(x.size())

#define forr(i,a,b) for(int i=int(a);i<int(b);++i)
#define forn(i,n) forr(i,0,n)

using vi = vector<int>;
using vb = vector<bool>;
using vvi = vector<vi>;

// KUHN - Maximum Matching en grafo bipartito
// Complejidad: O(V * E)
// Uso:
// Kuhn kuhn(n, m); [0, n) -> [0, m)
// kuhn.add_edge(u, v); // u del lado izquierdo, v del derecho
// int ans = kuhn.max_matching();
// kuhn.mt[v] = vertice izquierdo emparejado con v, o -1

struct Kuhn {
    int n, m; vvi g;
    vi mt; vb vis;
    Kuhn(int n, int m): n(n), m(m), g(n), mt(m), vis(n) {}
    void add_edge(int u, int v){
        g[u].pb(v);
    }
    bool dfs(int v){
        if(vis[v]) return false;
        vis[v] = true;
        for(int u : g[v]){
            if(mt[u] == -1 || dfs(mt[u])){
                mt[u] = v;
                return true;
            }
        }
        return false;
    }
    int max_matching(){
        fill(all(mt), -1);
        int ans = 0;
        forn(v, n){
            fill(all(vis), false);
            ans += dfs(v);
        }
        return ans;
    }
};

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    Kuhn kuhn(5, 4);

    kuhn.add_edge(0, 0);
    kuhn.add_edge(0, 1);
    kuhn.add_edge(1, 0);
    kuhn.add_edge(2, 1);
    kuhn.add_edge(2, 2);
    kuhn.add_edge(3, 2);
    kuhn.add_edge(3, 3);
    kuhn.add_edge(4, 3);

    int ans = kuhn.max_matching();

    cout << "Matching maximo: " << ans << endl;

    cout << "Parejas:" << endl;

    forn(v, kuhn.m){
        if(kuhn.mt[v] != -1) cout << "Izquierda " << kuhn.mt[v] << " - Derecha " << v << endl;
    }
    /*
    Un matching máximo posible:
    1 - 0
    0 - 1
    2 - 2
    3 - 3
    El vértice 4 queda libre.
    Matching máximo = 4
    */
    return 0;
}