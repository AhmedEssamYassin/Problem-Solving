#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

void primeFactorize(ll N, vector<ll> &primeFactors)
{
    while (!(N & 1))
        primeFactors.push_back(2), N >>= 1;
    for (ll i{3}; i * i <= N; i += 2)
    {
        while (N % i == 0)
            primeFactors.push_back(i), N /= i;
    }
    if (N > 1) // N is prime
        primeFactors.push_back(N);
    sort(primeFactors.begin(), primeFactors.end());
}

// As a rule of thumb, if you inevitably generate all factors, use sqrt(N) factorization.
void getAllFactors(ll N, vector<ll> &factors)
{
    vector<ll> primeFactors;
    primeFactorize(N, primeFactors);
    factors.push_back(1);
    for (const ll &p : primeFactors)
    {
        ll len = factors.size();
        for (ll i{}; i < len; i++)
            factors.push_back(p * factors[i]);

        set<ll> distinctDivisors(factors.begin(), factors.end());
        factors.clear();
        for (const ll &it : distinctDivisors)
            factors.push_back(it);
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
    int t = 1;
    ll N;
    cin >> t;
    while (t--)
    {
    }
    return 0;
}
