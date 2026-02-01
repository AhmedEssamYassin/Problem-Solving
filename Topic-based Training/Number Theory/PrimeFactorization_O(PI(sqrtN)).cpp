#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

vector<int> Prime, LPF;
bitset<1000001> isPrime;

void linearSieveOfEratosthenes(int N)
{
    isPrime.set(); // Initially Assuming all numbers to be primes
    LPF.resize(N + 1);
    isPrime[0] = isPrime[1] = 0; // 0 and 1 are NOT primes
    for (long long i{2}; i <= N; i++)
    {
        if (isPrime[i])
        {
            Prime.push_back(i);
            LPF[i] = i; // The least prime factor of a prime number is itself
        }
        for (long long j{}; j < (int)Prime.size() && i * Prime[j] <= N && Prime[j] <= LPF[i]; j++)
        {
            isPrime[i * Prime[j]] = 0; // Crossing out all the multiples of prime numbers
            LPF[i * Prime[j]] = Prime[j];
        }
    }
}
static int autoCall = (linearSieveOfEratosthenes(1000000), 0);

void primeFactorize(ll x, map<ll, ll> &mp)
{
    for (const ll &p : Prime)
    {
        if (p * p > x)
            break;
        while (x % p == 0)
            mp[p]++, x /= p;
    }
    if (x > 1)
        mp[x]++;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifdef LOCAL
    freopen("input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif
    int t = 1;
    ll N;
    cin >> t;
    while (t--)
    {
    }
    return 0;
}
