#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

const ll mod = 1e9 + 7;

#define double_size_t std::conditional_t<(mod > (1LL << 31)), __int128_t, long long>
inline ll add64(const ll &a, const ll &b)
{
    double_size_t res = double_size_t(a) + b;
    if (res >= mod)
        res -= mod;
    return res;
}

inline ll sub64(const ll &a, const ll &b)
{
    double_size_t res = double_size_t(a) - b;
    if (res < 0)
        res += mod;
    if (res >= mod)
        res -= mod;
    return res;
}

inline ll mult64(const ll &a, const ll &b)
{
    return double_size_t(a) * b % mod;
}

ll modPow(ll N, ll power)
{
    if (N % mod == 0 || N == 0)
        return 0;
    if (N == 1 || power == 0)
        return 1;

    ll res{1};
    while (power)
    {
        if (power & 1)
            res = mult64(res, N);
        N = mult64(N, N);
        power >>= 1;
    }
    return res;
}

vector<vector<int>> memo;
ll memoDP(const vector<vector<char>> &grid, int i, int j)
{
    if (i >= grid.size() || j >= grid[0].size())
        return 0; // Invalid move

    if (grid[i][j] == '#') // Wall
        return 0;          // Invalid move

    if (i == grid.size() - 1 && j == grid[0].size() - 1)
        return 1;

    int &ret = memo[i][j];
    if (~ret)
        return ret;
    ll goRight = memoDP(grid, i, j + 1);
    ll goDown = memoDP(grid, i + 1, j);
    return ret = add64(goRight, goDown);
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
    // cin >> t;
    while (t--)
    {
        int H, W;
        cin >> H >> W;
        vector<vector<char>> grid(H, vector<char>(W));
        for (int i{}; i < H; i++)
        {
            for (int j{}; j < W; j++)
                cin >> grid[i][j];
        }
        memo.assign(H + 1, vector<int>(W + 1, -1));
        cout << memoDP(grid, 0, 0);
    }
    return 0;
}
