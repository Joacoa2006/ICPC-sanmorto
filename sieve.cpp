#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

vector<bool> sieve(int n){
    vector<bool> prime(n + 1, true);

    prime[0] = prime[1] = false;

    for(int i = 2; i * i <= n; i++){
        if(!prime[i]) continue;

        for(int j = i * i; j <= n; j += i) prime[j] = false;
    }

    return prime;
} // O(n log log n)

int main(){
    int n = 200000;

    vector<bool> prime = sieve(n);

    cout << prime[2] << '\n';   // 1
    cout << prime[10] << '\n';  // 0
    cout << prime[17] << '\n';  // 1
}

