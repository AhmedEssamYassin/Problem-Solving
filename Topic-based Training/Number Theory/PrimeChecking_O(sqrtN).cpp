#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

bool isPrime(ll N)
{
    if (N < 2) // 0 or 1
        return false;
    if (N < 4) // 2 or 3
        return true;
    for (ll i = 2; i * i <= N; i++)
    {
        if (N % i == 0)
            return false;
    }
    return true;
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
