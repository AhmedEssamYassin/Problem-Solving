#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

vector<ll> Prime, LPF;
void linearSieveOfEratosthenes(int N)
{
    bitset<1000001> isPrime;
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
}
static int autoCall = (linearSieveOfEratosthenes(1000000), 0);

template <typename T>
T mult64(const T &a, const T &b, T mod)
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

template <typename T>
bool checkComposite(T N, T a, T d, int s)
{
    T X = modPow(a, d, N);
    if (X == 1 || X == N - 1)
        return false; // Not composite

    for (int r = 1; r < s; r++)
    {
        X = mult64(X, X, N);
        if (X == 1 || X == N - 1)
            return false; // Not composite
    }
    return true; // Composite
}

template <typename T>
bool Miller_Rabin(T N, int K = 5) // k is the number of trials (bases). If k increases the accuracy increases
{
    T d = N - 1;
    int s{};
    while (~s & 1)
        d >>= 1, ++s;

    for (const T &a : {11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47})
    {
        if (N == a)
            return true;
        if (checkComposite(N, a, d, s))
            return false;
    }
    return true;
}

template <typename T>
bool isPrime(T N)
{
    if (N < 2)
        return false;

    if (N <= 3)
        return true;
    if (N == 5 || N == 7)
        return true;

    if (!(N & 1) || N % 3 == 0 || N % 5 == 0 || N % 7 == 0)
        return false;

    return Miller_Rabin(N);
}

ll countDivisors(ll N)
{
    ll cnt = 1;
    for (const ll &p : Prime)
    {
        if (p * p * p > N)
            break;
        ll power{};
        while (N % p == 0)
            power++, N /= p;
        cnt *= (power + 1);
    }
    ll sqrtN = sqrtl(N);
    if (isPrime(N)) // N = p
        cnt *= 2;
    else if (sqrtN * sqrtN == N && isPrime(sqrtN)) // N = p²
        cnt *= 3;
    else if (N > 1) // Composite --> p * q
        cnt *= 4;
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
    // cin >> t;
    while (t--)
    {
        cin >> N;
        cout << countDivisors(N);
    }
    return 0;
}
