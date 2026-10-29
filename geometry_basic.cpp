#include <bits/stdc++.h>

using namespace std;

#define pb push_back
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x.size())
#define forr(i,a,b) for(int i=int(a);i<int(b);++i)
#define forn(i,n) forr(i,0,n)

typedef long long ll;
typedef long double ld;

const ld EPS = 1e-12;
const ld PI = acosl(-1.0L);

using T = ll;

struct pto {
    T x, y;

    pto(): x(0), y(0) {}
    pto(T x, T y): x(x), y(y) {}

    pto operator+(pto p) const { return {x+p.x, y+p.y}; }
    pto operator-(pto p) const { return {x-p.x, y-p.y}; }
    pto operator*(T k) const { return {x*k, y*k}; }
    T operator*(pto p) const { return x*p.x + y*p.y; } // dot
    T operator^(pto p) const { return x*p.y - y*p.x; } // cross
    bool operator==(pto p) const { return x==p.x && y==p.y; }
    bool operator<(pto p) const {
        return x!=p.x ? x<p.x : y<p.y;
    }
    ld norm() const {
        return sqrtl((ld)x*x + (ld)y*y);
    }
    ld dist(pto p) const {
        return (*this-p).norm();
    }
};

T cross(pto a, pto b, pto c){
    // Dada la recta a -> b dice donde está el punto c.
    // >0 izquierda; <0 derecha; =0 colinear
    return (b-a)^(c-a);
}

int orient(pto a, pto b, pto c){
    // Lo anterior normalizado 1, 0, -1
    T v = cross(a,b,c);
    return (v>0)-(v<0);
}

bool onSegment(pto p, pto a, pto b){
    // true si el punto p está sobre el segmento ab
    return cross(a,b,p)==0 &&
           min(a.x,b.x)<=p.x && p.x<=max(a.x,b.x) &&
           min(a.y,b.y)<=p.y && p.y<=max(a.y,b.y);
}

bool intersect(pto a, pto b, pto c, pto d){
    // true si los segmentos ab, cd se intersecan
    int x1=orient(a,b,c), x2=orient(a,b,d);
    int x3=orient(c,d,a), x4=orient(c,d,b);
    if(x1*x2<0 && x3*x4<0) return true;
    return (x1==0 && onSegment(c,a,b)) ||
           (x2==0 && onSegment(d,a,b)) ||
           (x3==0 && onSegment(a,c,d)) ||
           (x4==0 && onSegment(b,c,d));
}

T signedArea2(const vector<pto>& p){
    // area >0 puntos antihorarios. Caso contrario puntos horarios
    T res=0;
    forn(i,sz(p))
        res += p[i] ^ p[(i+1)%sz(p)];
    return res;
}

T area2(const vector<pto>& p){
    return llabs(signedArea2(p));
}

ld area(const vector<pto>& p){
    return (ld)area2(p)/2.0L;
}

vector<pto> convexHull(vector<pto> p){
    // Devuelve los puntos que forman la capsula convexa
    // Para mantener los puntos colineares de los bordes cambiar <=0 por <0
    sort(all(p));
    p.erase(unique(all(p)), p.end());
    if(sz(p)<=1) return p;
    vector<pto> h;
    for(auto x:p){
        while(sz(h)>=2 && cross(h[sz(h)-2], h.back(), x)<=0) // <0
            h.pop_back();
        h.pb(x);
    }
    int lower = sz(h);
    for(int i=sz(p)-2;i>=0;i--){
        auto x=p[i];
        while(sz(h)>lower && cross(h[sz(h)-2], h.back(), x)<=0) // <0
            h.pop_back();
        h.pb(x);
    }
    h.pop_back();
    return h;
}

// 0 afuera, 1 adentro, 2 borde
// Verifica si el punto está dentro del polígono simple
int inPolygon(const vector<pto>& p, pto q){
    bool in=false;
    forn(i,sz(p)){
        pto a=p[i], b=p[(i+1)%sz(p)];
        if(onSegment(q,a,b)) return 2;
        if((a.y>q.y)!=(b.y>q.y)){
            T cr=cross(a,b,q);
            if((b.y>a.y && cr>0) ||
               (b.y<a.y && cr<0))
                in=!in;
        }
    }
    return in;
}

struct ptd {
    ld x, y;

    ptd(): x(0), y(0) {}
    ptd(ld x, ld y): x(x), y(y) {}
    ptd(pto p): x(p.x), y(p.y) {}

    ptd operator+(ptd p) const { return {x+p.x, y+p.y}; }
    ptd operator-(ptd p) const { return {x-p.x, y-p.y}; }
    ptd operator*(ld k) const { return {x*k, y*k}; }
    ptd operator/(ld k) const { return {x/k, y/k}; }
    ld operator*(ptd p) const { return x*p.x + y*p.y; }
    ld operator^(ptd p) const { return x*p.y - y*p.x; }
    ld norm2() const { return (*this)*(*this); }
    ld norm() const { return sqrtl(norm2()); }
    ld dist(ptd p) const {
        return (*this-p).norm();
    }
};

struct line {
    ptd p, v; // p + t*v
    line() {}
    line(ptd p, ptd v): p(p), v(v) {}
};

line through(ptd a, ptd b){
    // Devuelve la recta que pasa por a y b
    return {a,b-a};
}

bool lineInter(line a, line b, ptd &res){
    // Devuelve true si las rectas a y b se intersecan. En dicho caso se guarda en res la intersección
    ld d = a.v ^ b.v;
    if(fabsl(d)<EPS) return false;
    res = a.p + a.v * (((b.p-a.p)^b.v)/d);
    return true;
}

ptd projection(line l, ptd q){
    // Proyeccion q en l
    return l.p + l.v * (((q-l.p)*l.v)/(l.v*l.v));
}

ld distPointLine(line l, ptd q){
    // Distancia de q a la recta l
    return fabsl(l.v^(q-l.p))/l.v.norm();
}

ld distPointSegment(ptd q, ptd a, ptd b){
    // Distancia de q al segmento ab
    ptd v=b-a;
    if(v.norm2()<EPS)
        return q.dist(a);
    ld t=((q-a)*v)/v.norm2();
    t=max((ld)0.0L,min((ld)1.0L,t));
    return q.dist(a+v*t);
}
