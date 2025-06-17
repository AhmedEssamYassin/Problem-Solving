#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"
// This INF is related to the constraints on inverse ETF, it depends on the problem.
#define INF 200000000LL

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

// As a rule of thumb, if you inevitably generate all factors, use sqrt(N) factorization.
template <typename T>
void getAllFactors(T N, vector<T> &factors)
{
    for (ll d = 1; d * d <= N; d++)
    {
        if (N % d == 0)
        {
            ll f1 = d;
            ll f2 = N / d;
            factors.push_back(f1);
            if (f2 != f1)
                factors.push_back(f2);
        }
    }
}

// Inverse Euler Totient Function
vector<int> divP;
template <typename T>
T solve(T phi, T Num, T pos)
{
    if (phi == 1)
        return Num;
    else if (pos == divP.size())
        return INF;
    else
    {
        T P = divP[pos];
        T res = solve(phi, Num, pos + 1);
        if (phi % (P - 1) == 0)
        {
            T remPhi = phi / (P - 1);
            T curNum = Num * P;
            res = min(res, solve(remPhi, curNum, pos + 1));
            while (remPhi % P == 0)
            {
                remPhi /= P, curNum *= P;
                res = min(res, solve(remPhi, curNum, pos + 1));
            }
        }
        return res;
    }
}
template <typename T>
T inversePhi(T phi)
{
    if (phi == 1)
        return 1;
    if (phi & 1)
        return INF;
    divP.clear();
    vector<T> allFactors;
    getAllFactors(phi, allFactors);
    for (T &factor : allFactors)
    {
        if (isPrime(factor + 1))
            divP.push_back(factor + 1);
    }
    return solve(phi, T(1), T(1));
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
        ll invPhi = inversePhi(N);
        if (invPhi == INF)
            cout << -1 << endl;
        else
            cout << invPhi << endl;
    }
    return 0;
}
