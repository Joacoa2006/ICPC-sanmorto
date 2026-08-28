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
using vl = vector<ll>;


// MO'S ALGORITHM
// Queries offline sobre rangos [l, r)
// Complejidad: O((n + q) * sqrt(n)) si add/remove son O(1)
// En cada problema adaptar: init(), add(i), remove(i), get_ans()

struct Query{
    int l, r, id;
};
int n, q, B;
vi a;
vector<Query> queries;
vl ans;

// -------------------------------------------------
// PARTE ESPECIFICA DEL PROBLEMA
// Cantidad de valores distintos en el rango actual
// -------------------------------------------------

vi freq;
int distinct;
void init(){
    distinct = 0;
}
void add(int i){
    if(freq[a[i]]++ == 0) distinct++;
}
void remove(int i){
    if(--freq[a[i]] == 0) distinct--;
}
ll get_ans(){
    return distinct;
}

// -------------------------------------------------
// MO
// -------------------------------------------------

bool qcomp(const Query &a, const Query &b){
    int ba = a.l / B;
    int bb = b.l / B;
    if(ba != bb) return ba < bb;
    if(ba & 1) return a.r > b.r;
    return a.r < b.r;
}

void mos(){
    B = max(1, (int)sqrt(n));
    sort(all(queries), qcomp);
    int l = 0, r = 0; // [l, r)
    init();
    for(auto qu : queries){
        while(l > qu.l) add(--l);
        while(r < qu.r) add(r++);
        while(l < qu.l) remove(l++);
        while(r > qu.r) remove(--r);
        ans[qu.id] = get_ans();
    }
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    cin >> n >> q;
    a.resize(n);
    forn(i, n) cin >> a[i];
    
    vi aux = a; // Compresión de coordenadas
    sort(all(aux));
    aux.erase(unique(all(aux)), aux.end());
    forn(i, n) a[i] = lower_bound(all(aux), a[i]) - aux.begin();
    freq.assign(sz(aux), 0);

    // Queries
    queries.resize(q);
    ans.resize(q);
    forn(i, q){
        int l, r;
        cin >> l >> r; --l;
        // Input [l,r] 1-based. Mo usa [l,r) 0-based
        queries[i] = {l, r, i};
    }
    mos();
    forn(i, q)cout << ans[i] << "\n";
}