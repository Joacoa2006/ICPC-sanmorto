#include <bits/stdc++.h>

using namespace std;

#define forr(i,a,b) for(int i=int(a);i<int(b);++i)
#define forn(i,n) forr(i,0,n)
#define dforr(i,a,b) for(int i = int(b)-1;i>=int(a);i--)
#define dforn(i,n) dforr(i,0,n)

using vi = vector<int>;
using vvi = vector<vi>;

int n, LOG;
vvi adj, up;
vi depth;

void dfs(int v, int p){
    up[v][0] = p;

    forr(j, 1, LOG) up[v][j] = up[up[v][j-1]][j-1];

    for(int u : adj[v]){
        if(u == p) continue;
        depth[u] = depth[v] + 1;
        dfs(u, v);
    }
}

void build_lca(int root = 1){ // O(n log n)
    LOG = 1;
    while((1 << LOG) <= n) LOG++;

    depth.assign(n + 1, 0);
    up.assign(n + 1, vi(LOG));

    dfs(root, root);
}

int jump(int v, int k){ // O(log n)
    forn(j, LOG)
        if(k & (1 << j))
            v = up[v][j];

    return v;
}

bool is_ancestor(int u, int v){ // O(log n)
    if(depth[u] > depth[v]) return false;
    return jump(v, depth[v] - depth[u]) == u;
}

int lca(int u, int v){ // O(log n)
    if(depth[u] < depth[v]) swap(u, v);

    u = jump(u, depth[u] - depth[v]);

    if(u == v) return u;

    dforn(j, LOG){
        if(up[u][j] != up[v][j]){
            u = up[u][j];
            v = up[v][j];
        }
    }

    return up[u][0];
}

int dist(int u, int v){ // O(log n)
    int w = lca(u, v);
    return depth[u] + depth[v] - 2 * depth[w];
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    n = 9;
    adj.assign(n + 1, {});

    auto agregar = [&](int u, int v){
        adj[u].push_back(v);
        adj[v].push_back(u);
    };

    agregar(1, 2);
    agregar(1, 3);
    agregar(2, 4);
    agregar(2, 5);
    agregar(3, 6);
    agregar(3, 7);
    agregar(6, 8);
    agregar(6, 9);

    /*
                1
              /   \
             2     3
            / \   / \
           4   5 6   7
                / \
               8   9
    */

    build_lca(1);

    cout << "LCA\n";
    cout << lca(4, 5) << '\n'; // 2
    cout << lca(4, 8) << '\n'; // 1
    cout << lca(8, 9) << '\n'; // 6
    cout << lca(6, 9) << '\n'; // 6
    cout << lca(8, 5) << '\n'; // 1

    cout << "\nANCESTOR\n";
    cout << is_ancestor(1, 9) << '\n'; // 1
    cout << is_ancestor(3, 8) << '\n'; // 1
    cout << is_ancestor(6, 8) << '\n'; // 1
    cout << is_ancestor(2, 8) << '\n'; // 0
    cout << is_ancestor(8, 6) << '\n'; // 0

    cout << "\nJUMP\n";
    cout << jump(8, 1) << '\n'; // 6
    cout << jump(8, 2) << '\n'; // 3
    cout << jump(8, 3) << '\n'; // 1
    cout << jump(8, 10) << '\n'; // 1, porque root apunta a sí mismo

    cout << "\nDIST\n";
    cout << dist(4, 5) << '\n'; // 2
    cout << dist(4, 8) << '\n'; // 5
    cout << dist(8, 9) << '\n'; // 2
    cout << dist(7, 9) << '\n'; // 3
}