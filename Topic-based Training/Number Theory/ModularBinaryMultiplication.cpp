#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

template <typename T>
T mult128(T a, T b, T mod)
{
    T result = 0; // Initialize result

    for (a %= mod; b > 0; a <<= 1, b >>= 1)
    {
        if (a >= mod)
            a -= mod;
        if (b & 1) // b is ODD
        {
            result += a;
            if (result >= mod)
                result -= mod;
        }
    }
    return result;
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
