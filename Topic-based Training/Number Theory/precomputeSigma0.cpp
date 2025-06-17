#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

vector<int> cntDivisors;
void precomputeSigma0(int N)
{
    cntDivisors.assign(N + 1, 0);
    for (int i = 1; i <= N; i++)
    {
        for (int j = i; j <= N; j += i)
            cntDivisors[j]++;
    }
}
static int autoCall = (precomputeSigma0(1000000), 0);

ll add64(const ll &a, const ll &b, const ll &mod)
{
    ll res = a + b;
    if (res >= mod)
        res -= mod;
    return res;
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
        ll a, b, c;
        const ll mod = (1LL << 30);
        cin >> a >> b >> c;
        ll Sum{};
        for (int i{1}; i <= a; i++)
        {
            for (int j{1}; j <= b; j++)
            {
                for (int k{1}; k <= c; k++)
                    Sum = add64(Sum, cntDivisors[i * j * k], mod);
            }
        }
        cout << Sum;
    }
    return 0;
}
