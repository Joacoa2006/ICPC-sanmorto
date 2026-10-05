#include <bits/stdc++.h>

using namespace std;

#define pb push_back
#define sz(x) (int)(x.size())

#define oper(a,b) min(a,b)
#define NEUT 1e18

typedef long long ll;

/*
vector<ll> st;      // datos
vector<int> L, R;   // punteros/indices de nodos
int root;           // indice de nodo
int p, l, r;        // posiciones
*/
struct PSTree {
    vector<ll> st; vector<int> L, R; int n;
    PSTree(int n): st(1, NEUT), L(1, 0), R(1, 0), n(n) {}
    int new_node(ll v, int l = 0, int r = 0){
        int k = sz(st);
        st.pb(v); L.pb(l); R.pb(r);
        return k;
    }
    int init(int s, int e, ll *a){
        if(s + 1 == e) return new_node(a[s]);
        int m = (s + e) / 2;
        int l = init(s, m, a);
        int r = init(m, e, a);
        return new_node(oper(st[l], st[r]), l, r);
    }
    int upd(int k, int s, int e, int p, ll v){
        int nk = new_node(st[k], L[k], R[k]);
        if(s + 1 == e){
            st[nk] = v;
            return nk;
        }
        int m = (s + e) / 2;
        if(p < m) L[nk] = upd(L[k], s, m, p, v);
        else R[nk] = upd(R[k], m, e, p, v);
        st[nk] = oper(st[L[nk]], st[R[nk]]);
        return nk;
    }
    ll query(int k, int s, int e, int a, int b){
        if(e <= a || b <= s) return NEUT;
        if(a <= s && e <= b) return st[k];
        int m = (s + e) / 2;
        return oper(query(L[k], s, m, a, b), query(R[k], m, e, a, b));
    }
    int init(ll *a){ return init(0, n, a); }
    int upd(int root, int p, ll v){ return upd(root, 0, n, p, v); }
    ll query(int root, int a, int b){ return query(root, 0, n, a, b); }
};
// USO: new_root = pst.upd(old_root, posicion, nuevo_valor);
// Hay que ir guardando los roots (indices)

int main(){
    vector<ll> v = {5, 8, 3, 10, 7};
    PSTree pst(sz(v));

    int r0 = pst.init(&v[0]);
    int r1 = pst.upd(r0, 2, 20);
    int r2 = pst.upd(r1, 0, 1);
    int r3 = pst.upd(r0, 4, 2);

    cout << pst.query(r0, 0, 5) << '\n';
    cout << pst.query(r1, 0, 5) << '\n';
    cout << pst.query(r2, 0, 5) << '\n';
    cout << pst.query(r3, 0, 5) << '\n';
}