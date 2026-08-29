#include <bits/stdc++.h>

using namespace std;

typedef long double ld;

bool ok(int n){
    // Chequea lo que el binary search necesite
}

// Binary Search [l,r) - ok: F F F T T T
int bs(int l, int r){
    while(l < r){
        int mid = l + (r-l)/2;
        if(ok(mid)) r = mid;
        else l = mid + 1;
    }
    return l;
}

// Binary Search real - ok: F F F T T T
ld bs(ld l, ld r, ld eps = 1e-9){
    while(r-l > eps){
        ld mid = (l+r)/2;
        if(ok(mid)) r = mid;
        else l = mid;
    }
    return r;
}
// Si el error permitido por la respuesta es 1e-6 -> eps = 1e-9
// Si es 1e-9 -> eps = 1e-12
// Con que eps sea una potencia mayor alcanza

int main(){
    int value;
    vector<vector<int>> vv;
    // Binary search en vector de vectores, devuelve un iterador al primer vector tal que su primer elemento sea >= que value
    auto it = lower_bound(vv.begin(), vv.end(), value,
        [](const vector<int> &a, int value){
            return a[0] < value;
        }
    );
    int ind = it - vv.begin();

    // Binary search en vector de vectores, devuelve un iterador al primer vector tal que su primer elemento sea > que value
    it = upper_bound(vv.begin(), vv.end(), value,
        [](int value, const vector<int> &a){
            return value < a[0];
        }
    );
    ind = it - vv.begin();

}
