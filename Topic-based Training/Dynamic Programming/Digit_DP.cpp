#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

// https://codeforces.com/contest/628/problem/D
// https://codeforces.com/contest/2121/problem/E
// https://codeforces.com/gym/100886/problem/G
// https://codeforces.com/contest/1036/problem/C
// https://codeforces.com/contest/1073/problem/E
// https://codeforces.com/group/o09Gu2FpOx/contest/541481/problem/L
// https://atcoder.jp/contests/abc154/tasks/abc154_e
// https://codeforces.com/contest/2132/problem/D
// https://codeforces.com/gym/104990/problem/B
// https://codeforces.com/contest/507/problem/D

int dp[10][2][2];
int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifdef LOCAL
    freopen("input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif
    int t = 1;
    cin >> t;
    while (t--)
    {
        string l, r;
        cin >> l >> r;
        memset(dp, -1, sizeof(dp));
        function<ll(ll, ll, ll)> slv = [&](ll i, ll td, ll tu) -> ll
        {
            if (i == l.size())
                return 0;
            int &ret = dp[i][td][tu];
            if (~ret)
                return ret;
            ret = 1e9;
            int lb = td ? l[i] - '0' : 0;
            int ub = tu ? r[i] - '0' : 9;
            for (int j = lb; j <= ub; j++)
            {
                int cost = 0;
                if (j == l[i] - '0')
                    cost++;
                if (j == r[i] - '0')
                    cost++;

                ret = min<ll>(ret, cost + slv(i + 1, td && j == lb, tu && j == ub));
            }
            return ret;
        };
        cout << slv(0, 1, 1) << endl;
    }
    return 0;
}
