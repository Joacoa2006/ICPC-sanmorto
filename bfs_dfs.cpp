#include <bits/stdc++.h>
using namespace std;

#define forr(i,a,b) for(int i=int(a);i<int(b);++i)
#define forn(i,n) forr(i,0,n)

using vi = vector<int>;
using vvi = vector<vi>;
using vb = vector<bool>;

int n;
vvi adj;

// ---------- DFS ---------- O(n + m)
vb visited;
vi tin, tout;
int timer;

void dfs(int v){
    tin[v] = timer++;
    visited[v] = true;
    for(int u : adj[v]){
        if(!visited[u]) dfs(u);
    }
    tout[v] = timer++;
}

// ---------- BFS ---------- O(n + m)
vi d, p;

void bfs(int s){
    d.assign(n + 1, -1);
    p.assign(n + 1, -1);
    queue<int> q;
    q.push(s);
    d[s] = 0;
    while(!q.empty()){
        int v = q.front();
        q.pop();
        for(int u : adj[v]){
            if(d[u] != -1) continue;
            d[u] = d[v] + 1;
            p[u] = v;
            q.push(u);
        }
    }
}


// Reconstruye camino desde el origen del último BFS hasta v.
// Si no existe camino devuelve vector vacío.
vi get_path(int v){
    if(d[v] == -1) return {};
    vi path;
    while(v != -1){
        path.push_back(v);
        v = p[v];
    }
    reverse(path.begin(), path.end());
    return path;
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> n;

    adj.assign(n + 1, {});

    int m;
    cin >> m;

    forn(i, m){
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // DFS
    visited.assign(n + 1, false);
    tin.assign(n + 1, 0);
    tout.assign(n + 1, 0);
    timer = 0;
    dfs(1);

    // BFS desde 1
    bfs(1);

    cout << "Distancia de 1 a n: " << d[n] << '\n';

    vi camino = get_path(n);

    for(int v : camino) cout << v << " ";
    cout << '\n';
}