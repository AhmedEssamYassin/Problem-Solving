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
    if (N % mod == 0 || N == 0)
        return 0;
    if (N == 1 || power == 0)
        return 1;
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

vector<ll> Prime, LPF;
bitset<10000001> isPrime;
vector<ll> cntPrimes;
void linearSieveOfEratosthenes(int N)
{
    isPrime.set(); // Initially Assuming all numbers to be primes
    LPF.resize(N + 1);
    isPrime[0] = isPrime[1] = 0; // 0 and 1 are NOT primes
    for (int i{2}; i <= N; i++)
    {
        if (isPrime[i])
        {
            Prime.push_back(i);
            LPF[i] = i; // The least prime factor of a prime number is itself
        }
        for (int j{}; j < (int)Prime.size() and i * Prime[j] <= N and Prime[j] <= LPF[i]; j++)
        {
            isPrime[i * Prime[j]] = 0; // Crossing out all the multiples of prime numbers
            LPF[i * Prime[j]] = Prime[j];
        }
    }
    cntPrimes.resize(N + 1);
    for (int i = 1; i <= N; i++)
        cntPrimes[i] = cntPrimes[i - 1] + isPrime[i];
}
static int autoCall = (linearSieveOfEratosthenes(1e7), 0);

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
    for (const int &p : Prime)
    {
        if (p > N)
            break;
        primeFactors[p] = expFactor(N, p);
    }
}

ll countDivisors(ll N, ll mod)
{
    ll cnt = 1;
    ll sqrtN = sqrt(N);

    // Handle primes <= sqrt(N) using Legendre's formula
    for (const ll &p : Prime)
    {
        if (p > sqrtN)
            break;
        cnt = mult64(cnt, expFactor(N, p) + 1, mod);
    }

    // Group primes with same N/p
    ll i = cntPrimes[sqrtN];
    while (true)
    {
        ll L = i;
        ll Q = N / Prime[L];
        if (Q == 0)
            break;
        ll R = cntPrimes[N / Q];
        cnt = mult64(cnt, modPow(Q + 1, R - L, mod), mod);
        i = R;
        if (i >= Prime.size())
            break;
    }

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
    ll N, M;
    cin >> t;
    while (t--)
    {
        cin >> N >> M;
        cout << countDivisors(N, M) << endl;
    }
    return 0;
}
