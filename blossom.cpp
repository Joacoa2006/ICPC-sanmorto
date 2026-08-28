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

struct Blossom { // O(n ^ 3)
    int n; vvi g;
    vi mt, p, base, q;
    vb used, blossom;

    Blossom(int n): n(n), g(n), mt(n), p(n), base(n), q(n), used(n), blossom(n) {}

    void add_edge(int u, int v){
        g[u].pb(v); g[v].pb(u);
    }

    int lca(int a, int b){
        vb vis(n, false);
        while(true){
            a = base[a];
            vis[a] = true;
            if(mt[a] == -1) break;
            a = p[mt[a]];
        }
        while(true){
            b = base[b];
            if(vis[b]) return b;
            b = p[mt[b]];
        }
    }

    void mark_path(int v, int b, int child){
        while(base[v] != b){
            blossom[base[v]] = blossom[base[mt[v]]] = true;
            p[v] = child;
            child = mt[v];
            v = p[mt[v]];
        }
    }

    int find_path(int root){
        fill(all(used), false);
        fill(all(p), -1);
        iota(all(base), 0);
        int qh = 0, qt = 0;
        q[qt++] = root;
        used[root] = true;
        while(qh < qt){
            int v = q[qh++];
            for(int u : g[v]){
                if(base[v] == base[u] || mt[v] == u) continue;
                if(u == root || (mt[u] != -1 && p[mt[u]] != -1)){
                    int cur = lca(v, u);
                    fill(all(blossom), false);
                    mark_path(v, cur, u);
                    mark_path(u, cur, v);
                    forn(i, n){
                        if(blossom[base[i]]){
                            base[i] = cur;
                            if(!used[i]){
                                used[i] = true;
                                q[qt++] = i;
                            }
                        }
                    }
                } else if(p[u] == -1){
                    p[u] = v;
                    if(mt[u] == -1) return u;
                    u = mt[u];
                    used[u] = true;
                    q[qt++] = u;
                }
            }
        }
        return -1;
    }

    int max_matching(){
        fill(all(mt), -1);
        int ans = 0;
        forn(v, n){
            if(mt[v] != -1) continue;
            int u = find_path(v);
            if(u == -1) continue;
            ans++;
            while(u != -1){
                int pv = p[u];
                int nxt = mt[pv];
                mt[u] = pv;
                mt[pv] = u;
                u = nxt;
            }
        }
        return ans;
    }
};

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n, m;
    cin >> n >> m;

    Blossom bl(n);

    forn(i, m){
        int u, v;
        cin >> u >> v; --u;--v;
        bl.add_edge(u, v);
    }

    int ans = bl.max_matching();
    cout << ans << endl;

    forn(i, n){ // Obtener las parejas
        if(bl.mt[i] != -1 && i < bl.mt[i]){
            cout << i << " " << bl.mt[i] << endl;
        }
    }
}