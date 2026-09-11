#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

// ------------------------------------------------------------
// EJEMPLO ACTUAL: Transformacion afín
// Query: suma
// Update: x -> a*x+b
//
// Permite:
// {1,x} -> sumar x
// {0,x} -> asignar x
// {x,0} -> multiplicar por x
// {a,b} -> affine general
// ------------------------------------------------------------

struct Node { // TODO
    ll val;
};
struct Lazy { // TODO
    ll a, b; // x -> a*x+b
}; // Informacion minima para representar un update pendiente.
const Node NEUT = {0};
const Lazy ID = {1, 0};
Node oper(Node x, Node y){
    return {x.val + y.val}; // TODO
}
// sum(a*x+b) = a*sum(x) + b*cantidad
Node apply(Lazy v, Node x, int s, int e){ // TODO
    return {v.a*x.val + v.b*(e-s)};
} // Aplica el lazy v a TODO el segmento [s,e). Debe ser O(1)
Lazy compose(Lazy v, Lazy u){ // TODO
    return {v.a * u.a, v.a * u.b + v.b};
} // Compone dos lazys. v(u(x))
struct STree {
    int n; vector<Node> st; vector<Lazy> lazy;
    STree(int n): n(n), st(4*n+5, NEUT), lazy(4*n+5, ID) {}
    void init(int k, int s, int e, int *a){
        lazy[k] = ID;
        if(s+1 == e){
            st[k] = {a[s]};
            return;
        }
        int m = (s+e)/2;
        init(2*k, s, m, a);
        init(2*k+1, m, e, a);
        st[k] = oper(st[2*k], st[2*k+1]);
    }
    void apply_node(int k, int s, int e, Lazy v){
        st[k] = apply(v, st[k], s, e);
        lazy[k] = compose(v, lazy[k]);
    }
    void push(int k, int s, int e){
        if(lazy[k].a == ID.a && lazy[k].b == ID.b) return;
        if(s+1 < e){
            int m = (s+e)/2;
            apply_node(2*k, s, m, lazy[k]);
            apply_node(2*k+1, m, e, lazy[k]);
            // Si el lazy depende de la posicion puede ser necesario:
            // apply_node(2*k+1, m, e, shift(lazy[k], m-s));
            // Tambien podría aparecer en el update
        }
        lazy[k] = ID;
    }
    void upd(int k, int s, int e, int a, int b, Lazy v){
        if(s >= b || e <= a) return;
        if(s >= a && e <= b){
            apply_node(k, s, e, v);
            return;
        }
        push(k, s, e);
        int m = (s+e)/2;
        upd(2*k, s, m, a, b, v);
        upd(2*k+1, m, e, a, b, v);
        st[k] = oper(st[2*k], st[2*k+1]);
    }
    Node query(int k, int s, int e, int a, int b){
        if(s >= b || e <= a) return NEUT;
        if(s >= a && e <= b) return st[k];
        push(k, s, e);
        int m = (s+e)/2;
        return oper(query(2*k, s, m, a, b), query(2*k+1, m, e, a, b));
    }
    void init(int *a){init(1, 0, n, a);}
    void upd(int a, int b, Lazy v){upd(1, 0, n, a, b, v);}
    Node query(int a, int b){return query(1, 0, n, a, b);}
};

int main(){
    int n, l, r, x, a, b; vector<int> v;
    STree st(n);
    st.init(&v[0]);

    st.upd(l, r, {1, x}); // +x
    st.upd(l, r, {0, x}); // set x
    st.upd(l, r, {x, 0}); // *x
    st.upd(l, r, {a, b}); // valor -> a*valor+b

    cout << st.query(l, r).val << '\n';
}