#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

template <typename T>
inline T mult64(const T &a, const T &b, T mod)
{
    return (__int128_t)a * b % mod;
}

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
    for (const ll &p : Prime)
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
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
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
