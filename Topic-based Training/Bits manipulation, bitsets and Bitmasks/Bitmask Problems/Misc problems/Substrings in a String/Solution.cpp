#include <bits/stdc++.h>
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
using namespace std;
#define ll long long int
#define endl "\n"

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifdef LOCAL
    freopen("input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif
    int t = 1;
    // cin >> t;
    while (t--)
    {
        constexpr int maxN = 100001;
        string str;
        cin >> str;
        int q;
        cin >> q;
        int n = str.length();

        vector<bitset<maxN>> b(26);
        for (int i = 0; i < n; i++)
            b[str[i] - 'a'].set(i);

        while (q--)
        {
            int type;
            cin >> type;
            if (type == 1)
            {
                int i;
                char c;
                cin >> i >> c;
                i--;
                b[str[i] - 'a'].reset(i);
                str[i] = c;
                b[str[i] - 'a'].set(i);
            }
            else
            {
                int l, r;
                string y;
                cin >> l >> r >> y;
                l--;
                r--;
                int m = y.length();

                if (r - l + 1 < m)
                {
                    cout << 0 << endl;
                    continue;
                }

                bitset<maxN> match;
                match.set();

                for (int i = 0; i < m; i++)
                    match &= (b[y[i] - 'a'] >> i);

                int len = r - m + 1 - l + 1;
                match >>= l;
                match <<= (maxN - len);

                cout << match.count() << endl;
            }
        }
    }
    return 0;
}