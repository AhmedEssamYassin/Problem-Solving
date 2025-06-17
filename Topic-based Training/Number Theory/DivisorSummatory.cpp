#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

// Calculates sum of Sigma0 of all numbers from 1 to N
// i.e., calculates the sum of count of divisors of all numbers from 1 to N
ll divisorSummatory(ll N)
{
    ll ans{};
    for (ll i{1}; i * i <= N; i++)
        ans += (N / i);
    ans *= 2;
    ll sqrtN = sqrtl(N);
    ans -= (sqrtN * sqrtN);
    return ans;
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
        cout << divisorSummatory(N) << endl;
    }
    return 0;
}
