#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

#define oper min
#define NEUT 1e9
// k = indice del nodo
// s = inicio del segmento
// e = final del segmento
// [s, e)
// a es el arreglo inicial
// upd: actualiza a[p] = v
// query: busca en el rango [a, b)
// Los valores de los indices se piensan en el rango [0, n - 1] -> valores del vector. Las posiciones tmb se piensan con base 0.
struct STree {
	vector<int> st; int n;
	STree(int n): st(4*n+5,NEUT), n(n) {}
	void init(int k, int s, int e, int *a){
		if(s+1==e){st[k]=a[s];return;}
		int m=(s+e)/2;
		init(2*k,s,m,a);init(2*k+1,m,e,a);
		st[k]=oper(st[2*k],st[2*k+1]);
	}
	void upd(int k, int s, int e, int p, int v){
		if(s+1==e){st[k]=v;return;}
		int m=(s+e)/2;
		if(p<m)upd(2*k,s,m,p,v);
		else upd(2*k+1,m,e,p,v);
		st[k]=oper(st[2*k],st[2*k+1]);
	}
	int query(int k, int s, int e, int a, int b){
		if(s>=b||e<=a)return NEUT;
		if(s>=a&&e<=b)return st[k];
		int m=(s+e)/2;
		return oper(query(2*k,s,m,a,b),query(2*k+1,m,e,a,b));
	}
	void init(int *a){init(1,0,n,a);}
	void upd(int p, int v){upd(1,0,n,p,v);}
	int query(int a, int b){return query(1,0,n,a,b);}
}; // usage: STree rmq(n);rmq.init(x);rmq.upd(i,v);rmq.query(s,e);

int main(){
    int n = 6;
    int a[] = {5, 2, 7, 1, 9, 3};
    STree st(n);
    st.init(a);
    cout << st.query(0, 6) << '\n'; // 1
    cout << st.query(1, 4) << '\n'; // 1
    cout << st.query(4, 6) << '\n'; // 3
    // Cambio a[3] = 8
    st.upd(3, 8);
    cout << st.query(0, 6) << '\n'; // 2
    cout << st.query(1, 4) << '\n'; // 2
    // Cambio a[1] = 10
    st.upd(1, 10);
    cout << st.query(0, 6) << '\n'; // 3
    cout << st.query(0, 3) << '\n'; // 5
}