#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

template <typename T>
inline T mult64(const T &a, const T &b, T mod)
{
    return (__int128_t)a * b % mod;
}

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

// Calculating the exponent of a prime `p` in N! (Legendre's Formula)
int sumOfBin(ll N, int base)
{
    int res{};
    while (N != 0)
    {
        res += (N % base);
        N /= base;
    }
    return res;
}
ll expFactor(ll N, int p)
{
    // ll exponent = (N - sumOfBin(N, p)) / (p - 1);
    ll exponent = 0;
    while ((N /= p) != 0)
        exponent += N;
    return exponent;
}

void factorialFactorize(ll N, map<ll, ll> &primeFactors)
{
    ll ans{1};
    for (const ll &p : primes)
    {
        if (p > N)
            break;
        primeFactors[p] = expFactor(N, p);
    }
}

ll countDivisors(ll N, ll mod)
{
    map<ll, ll> primeFactors;
    factorialFactorize(N, primeFactors);
    ll cnt{1};
    for (const auto &[p, exp] : primeFactors)
        cnt = mult64(cnt, exp + 1, mod);
    return cnt;
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
        cin >> N;
        cout << countDivisors(N, 1e9 + 7) << endl;
    }
    return 0;
}
