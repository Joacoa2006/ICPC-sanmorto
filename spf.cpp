#include <bits/stdc++.h>

using namespace std;

#define all(x) x.begin(), x.end()
#define sz(x) (int)(x.size())

#define forr(i,a,b) for(int i=int(a);i<int(b);++i)
#define forn(i,n) forr(i,0,n)

typedef long long ll;

// ----- Factorización y divisores simple ----- O(sqrt(n))
map<ll, int> factorizar(ll n){
    map<ll, int> f;

    for(ll p = 2; p * p <= n; p++){
        while(n % p == 0){
            f[p]++;
            n /= p;
        }
    }

    if(n > 1) f[n]++;

    return f;
}

vector<ll> divisores(ll n){
    vector<ll> divs;

    for(ll d = 1; d * d <= n; d++){
        if(n % d == 0){
            divs.push_back(d);

            if(d != n / d) divs.push_back(n / d);
        }
    }

    sort(all(divs));
    return divs;
}

// -------------------- SPF - Smallest Prime Factor -------------------

vector<int> spf;

// Construccion O(n log log n), como la criba
void build_spf(int n){
    spf.resize(n + 1);
    iota(all(spf), 0);

    for(int i = 2; i * i <= n; i++){
        if(spf[i] != i) continue;

        for(ll j = 1LL * i * i; j <= n; j += i)
            if(spf[j] == j)
                spf[j] = i;
    }
} 

// Factorizar en O(log n). Sirve para factorizar mucho
map<int,int> factorizar_spf(int n){
    map<int,int> f;

    while(n > 1){
        f[spf[n]]++;
        n /= spf[n];
    }

    return f;
}

// La complejidad depende de la cantidad de divisores.
vector<int> divisores_spf(int n){
    map<int, int> f = factorizar_spf(n);

    vector<int> divs = {1};

    for(auto [p, e] : f){
        int tam = divs.size();
        int pot = 1;

        forr(k, 1, e + 1){
            pot *= p;

            forn(i, tam) divs.push_back(divs[i] * pot);
        }
    }

    sort(all(divs));
    return divs;
}

// O(1) con spf. 2 <= n <= LIM
bool es_primo(int n){
    return n >= 2 && spf[n] == n;
}

int main(){
    // Sin preprocesamiento
    auto f = factorizar(360);
    auto d = divisores(360);

    for(auto [p, e] : f)
        cout << p << "^" << e << " ";
    cout << '\n';

    for(ll x : d)
        cout << x << " ";
    cout << '\n';


    // Con SPF
    const int LIM = 200000;
    build_spf(LIM);

    cout << es_primo(97) << '\n';  // 1
    cout << es_primo(100) << '\n'; // 0

    auto f2 = factorizar_spf(360);
    auto d2 = divisores_spf(360);

    for(int x : d2)
        cout << x << " ";
    cout << '\n';
}