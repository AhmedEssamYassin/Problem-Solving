#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

template <typename T>
inline T mult64(const T &a, const T &b, T mod)
{
    return (__int128_t)a * b % mod;
}

template <typename T>
T modPow(T N, T power, T mod)
{
    T res{1};
    while (power)
    {
        if (power & 1)
            res = mult64(res, N, mod);
        N = mult64(N, N, mod);
        power >>= 1;
    }
    return res;
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
    const ll p = 1e9 + 7;
    // cin >> t;
    while (t--)
    {
        cin >> N;
        vector<ll> prime(N), exp(N);
        for (int i{}; i < N; i++)
            cin >> prime[i] >> exp[i];

        ll cntDivisors{1};
        ll cntDiv2{1};
        ll sumDivisors{1};
        ll productDivisors{1};
        for (int i{}; i < N; i++)
        {
            // Counting divisors
            cntDivisors = cntDivisors * (exp[i] + 1) % p;

            // Sum of divisors
            sumDivisors = (sumDivisors * (modPow(prime[i], exp[i] + 1, p) - 1) % p * modPow(prime[i] - 1, p - 2, p)) % p;

            // Product of divisors
            productDivisors = (modPow(productDivisors, exp[i] + 1, p) * modPow(modPow(prime[i], exp[i] * (exp[i] + 1) / 2, p), cntDiv2, p)) % p;
            cntDiv2 = (cntDiv2 * (exp[i] + 1)) % (p - 1);
        }
        cout << cntDivisors << " " << sumDivisors << " " << productDivisors;
    }
    return 0;
}
