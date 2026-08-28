#include <bits/stdc++.h>

using namespace std;

#define pb push_back
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x.size())

#define nn cout << '\n'

#define forr(i,a,b) for(int i = int(a); i < int(b); ++i)
#define forn(i,n) forr(i,0,n)

typedef long long ll;

const int MAXN = 2e5 + 5;

// DSU / Union Find
int uf[MAXN];
void uf_init(){ 
    memset(uf, -1, sizeof(uf)); 
}
int uf_find(int x){
    return uf[x] < 0 ? x : uf[x] = uf_find(uf[x]);
}
bool uf_join(int x, int y){
    x = uf_find(x);
    y = uf_find(y);
    if(x == y) return false;
    if(uf[x] > uf[y]) swap(x, y);
    uf[x] += uf[y];
    uf[y] = x;
    return true;
}
int uf_size(int x){
    return -uf[uf_find(x)];
}

// KRUSKAL - Minimum Spanning Tree
// Complejidad: O(m log m)

struct Edge{
    int u, v; ll w;
    bool operator<(const Edge &o) const{
        return w < o.w;
    }
};
vector<Edge> mst;

ll kruskal(int n, vector<Edge> &edges){
    sort(all(edges));
    uf_init();
    mst.clear();
    ll cost = 0;
    for(auto e : edges){
        if(uf_join(e.u, e.v)){
            cost += e.w; mst.pb(e);
        }
    }
    return sz(mst) == n - 1 ? cost : -1;
}


int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n, m;
    cin >> n >> m;

    vector<Edge> edges(m);

    for(auto &[u, v, w] : edges){
        cin >> u >> v >> w;
        --u;
        --v;
    }

    ll ans = kruskal(n, edges);

    cout << "Costo MST: " << ans;
    nn;

    cout << "Aristas:";
    nn;

    for(auto [u, v, w] : mst){
        cout << u << " " << v << " " << w;
        nn;
    }

    return 0;
}