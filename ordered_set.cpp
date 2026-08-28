#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

// ORDERED SET
// No admite repetidos
// O(log n) por operacion
// s.order_of_key(x)   -> cantidad de elementos < x
// *s.find_by_order(k) -> k-esimo elemento, 0-based

typedef tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> ordered_set;

// ORDERED MULTISET
// Admite repetidos usando {valor, id_unico}
// O(log n) por operacion
// s.order_of_key({x, INT_MIN}) -> cantidad de valores < x
// s.order_of_key({x, INT_MAX}) -> cantidad de valores <= x

typedef tree<pair<int,int>, null_type, less<pair<int,int>>, rb_tree_tag, tree_order_statistics_node_update> ordered_multiset;

int main(){
    ordered_set s;
    s.insert(2); s.insert(3);
    s.insert(7); s.insert(9);
    // s = {2, 3, 7, 9}
    
    s.erase(3);
    // s = {2, 7, 9}

    if(s.find(7) != s.end()) cout << "7 esta\n";

    auto it = s.lower_bound(5);
    if(it != s.end()) cout << *it << "\n";        // 7

    it = s.upper_bound(7);
    if(it != s.end()) cout << *it << "\n";        // 9

    // Cantidad de elementos estrictamente menores que x
    cout << s.order_of_key(7) << "\n";   // 1 -> {2}
    cout << s.order_of_key(8) << "\n";   // 2 -> {2,7}
    cout << s.order_of_key(100) << "\n"; // 3

    // k-esimo elemento, 0-based
    cout << *s.find_by_order(0) << "\n"; // 2
    cout << *s.find_by_order(1) << "\n"; // 7
    cout << *s.find_by_order(2) << "\n"; // 9

    // Ojo: si k >= size(), devuelve end()
    if(s.find_by_order(3) == s.end()) cout << "No existe\n";

    // Cantidad de elementos < x
    int x = 8;
    int menores = s.order_of_key(x);

    // Cantidad de elementos <= x
    int menores_iguales = s.order_of_key(x + 1);

    // Cantidad de elementos >= x
    int mayores_iguales = s.size() - s.order_of_key(x);

    // Cantidad de elementos > x
    int mayores = s.size() - s.order_of_key(x + 1);

    // Cantidad de elementos en [l, r]
    int l = 2, r = 8;
    int en_rango = s.order_of_key(r + 1) - s.order_of_key(l); // {2,7} -> 2

    ordered_multiset s1;
    int id = 0;
    s1.insert({5, id++});
    s1.insert({5, id++});
    s1.insert({5, id++});
    s1.insert({10, id++});

    // Cantidad de elementos < x
    s1.order_of_key({x, INT_MIN});

    // Cantidad de elementos <= x
    s1.order_of_key({x, INT_MAX});

    // Cantidad de elementos en [l, r]
    int cantidad = s1.order_of_key({r, INT_MAX}) - s1.order_of_key({l, INT_MIN});
}