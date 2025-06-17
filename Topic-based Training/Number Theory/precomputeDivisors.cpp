#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

// Works fine for N <= 1e5
vector<vector<int>> divisors;
void precomputeDivisors(int N)
{
    divisors.resize(N + 1);
    for (int i = 1; i <= N; i++)
    {
        for (int j = i; j <= N; j += i)
            divisors[j].push_back(i);
    }
}
static int autoCall = (precomputeDivisors(100000), 0);

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
    }
    return 0;
}
