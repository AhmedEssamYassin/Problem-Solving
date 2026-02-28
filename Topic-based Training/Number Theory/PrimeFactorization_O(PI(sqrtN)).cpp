#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

vector<int> primes, LPF;
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
            primes.push_back(i);
            LPF[i] = i; // The least prime factor of a prime number is itself
        }
        for (long long j{}; j < (int)primes.size() && i * primes[j] <= N && primes[j] <= LPF[i]; j++)
        {
            isPrime[i * primes[j]] = 0; // Crossing out all the multiples of prime numbers
            LPF[i * primes[j]] = primes[j];
        }
    }
}
static int autoCall = (linearSieveOfEratosthenes(1000000), 0);

void primeFactorize(ll x, map<ll, ll> &mp)
{
    for (const ll &p : primes)
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
