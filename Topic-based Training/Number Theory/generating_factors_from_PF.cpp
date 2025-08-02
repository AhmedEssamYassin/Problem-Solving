#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

void primeFactorize(ll N, map<ll, ll> &primeFactors)
{
    while (!(N & 1))
        primeFactors[2]++, N >>= 1;
    for (ll i{3}; i * i <= N; i += 2)
    {
        while (N % i == 0)
            primeFactors[i]++, N /= i;
    }
    if (N > 1) // N is prime
        primeFactors[N]++;
}

// As a rule of thumb, if you inevitably generate all factors, use sqrt(N) factorization.
void getAllFactors(ll N, vector<ll> &factors)
{
    factors = {1};
    map<ll, ll> freq;
    primeFactorize(N, freq);
    for (auto &[p, cnt] : freq)
    {
        vector<ll> temp;
        ll pw = 1;
        for (int i = 0; i <= cnt; i++, pw *= p)
        {
            for (const ll &f : factors)
                temp.push_back(f * pw);
        }
        factors.swap(temp);
    }
    sort(factors.begin(), factors.end());
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
    ll N, K;
    cin >> t;
    while (t--)
    {
        cin >> N >> K;
        vector<ll> allDivisors;
        getAllFactors(N, allDivisors);
        int pos = upper_bound(allDivisors.begin(), allDivisors.end(), K) - allDivisors.begin();
        pos--; // Get the position of the largest divisor less than K
        if (pos < 0)
            cout << N << endl;
        else
            cout << N / allDivisors[pos] << endl;
    }
    return 0;
}
