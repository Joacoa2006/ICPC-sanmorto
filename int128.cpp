#include <bits/stdc++.h>

using namespace std;

#define all(x) x.begin(), x.end()

string to_string(__int128_t value){
    if(value == 0) return "0";
 
    string s;
    bool neg = false;
 
    if(value < 0){
        neg = true;
        s += '0' + -(value%10);
        value/=-10;
    }
 
    while(value > 0){
        s+= '0' + (value%10);
        value/=10;
    }
 
    if(neg){
        s+='-';
    }
    reverse(all(s));
    return s;
}