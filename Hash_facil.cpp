vpll compute_hash(string const& s){
    const int p = 31;
    const int m = 999727999;
    ll hash_value = 0;
    ll p_pow = 1;
    vpll ans (SZ(s));
    int cont = 0;
    for(char c : s){
        hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
        ans[cont].fst = hash_value;
        ans[cont].snd = p_pow;
        cont++;
        p_pow = (p_pow * p) % m;
    }
    return ans;
}
