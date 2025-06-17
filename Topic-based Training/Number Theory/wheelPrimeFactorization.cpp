#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

void wheelPrimeFactorize(ll N, vector<ll> &factors)
{
    for (const int &d : {2, 3, 5})
    {
        while (N % d == 0)
        {
            factors.push_back(d);
            N /= d;
        }
    }
    static array<int, 8> increments = {4, 2, 4, 2, 4, 6, 2, 6};
    int i = 0;
    for (ll d = 7; d * d <= N; d += increments[i++])
    {
        while (N % d == 0)
        {
            factors.push_back(d);
            N /= d;
        }
        if (i == 8)
            i = 0;
    }
    if (N > 1)
        factors.push_back(N);
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
