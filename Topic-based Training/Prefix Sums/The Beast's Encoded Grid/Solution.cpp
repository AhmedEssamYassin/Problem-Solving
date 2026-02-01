#include <bits/stdc++.h>
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
		int n, m, k;
		cin >> n >> m >> k;
		vector<vector<char>> vc(n + 1, vector<char>(m + 1));
		for (int i{1}; i <= n; i++)
		{
			for (int j{1}; j <= m; j++)
				cin >> vc[i][j];
		}
		string str;
		cin >> str;
		ll F[26]{};
		for (const char &c : str)
			F[c - 'a']++;

		vector<vector<vector<ll>>> preSum(n + 1, vector<vector<ll>>(m + 1, vector<ll>(26)));
		for (int i{1}; i <= n; i++)
		{
			for (int j{1}; j <= m; j++)
			{
				for (int c = 0; c < 26; c++)
					preSum[i][j][c] = preSum[i - 1][j][c] + preSum[i][j - 1][c] - preSum[i - 1][j - 1][c];
				preSum[i][j][vc[i][j] - 'a']++;
			}
		}
		auto query = [&](int r1, int c1, int r2, int c2, int ch) -> ll
		{
			return preSum[r2][c2][ch] - preSum[r1 - 1][c2][ch] - preSum[r2][c1 - 1][ch] + preSum[r1 - 1][c1 - 1][ch];
		};

		int minSide = -1;
		int L = 1, R = min(n, m);
		while (L <= R)
		{
			int side = ((L + R) >> 1);
			bool found = false;
			for (int i = side; i <= n && !found; i++)
			{
				for (int j = side; j <= m && !found; j++)
				{
					bool ok = true;
					for (int ch = 0; ch < 26; ++ch)
					{
						if (query(i - side + 1, j - side + 1, i, j, ch) < F[ch])
						{
							ok = false;
							break;
						}
					}
					if (ok)
					{
						found = true;
						break;
					}
				}
			}
			if (found)
			{
				minSide = side;
				R = side - 1;
			}
			else
				L = side + 1;
		}

		if (minSide == -1)
			cout << -1;
		else
			cout << minSide * minSide;
	}
	return 0;
}