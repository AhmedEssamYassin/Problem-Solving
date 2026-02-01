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
	ll N, M, K;
	// cin >> t;
	while (t--)
	{
		cin >> N >> M >> K;
		vector<vector<char>> grid(N + 1, vector<char>(M + 1));
		for (int i{1}; i <= N; i++)
		{
			for (int j{1}; j <= M; j++)
				cin >> grid[i][j];
		}

		vector<vector<vector<ll>>> prefixSum3D(N + 1, vector<vector<ll>>(M + 1, vector<ll>(26, 0)));

		// Calculate the prefix sum for each character
		for (int i = 1; i <= N; i++)
		{
			for (int j = 1; j <= M; j++)
			{
				// Copy values from previous cells
				for (int k = 0; k < 26; k++)
					prefixSum3D[i][j][k] = prefixSum3D[i - 1][j][k] + prefixSum3D[i][j - 1][k] - prefixSum3D[i - 1][j - 1][k];
				// Increment the count for the current character
				prefixSum3D[i][j][grid[i][j] - 'A']++;
			}
		}

		auto countDistinctInSubGrid = [&](int x1, int y1, int x2, int y2)
		{
			ll distinct = 0;
			for (int k = 0; k < 26; k++)
			{
				ll count = prefixSum3D[x2][y2][k] - prefixSum3D[x2][y1 - 1][k] - prefixSum3D[x1 - 1][y2][k] + prefixSum3D[x1 - 1][y1 - 1][k];
				if (count > 0)
					distinct++;
			}
			return distinct;
		};

		auto checkSubGrid = [&](int N, int M, ll K, ll len) -> pair<int, int>
		{
			for (int i = 1; i + len - 1 <= N; i++)
			{
				for (int j = 1; j + len - 1 <= M; j++)
				{
					if (countDistinctInSubGrid(i, j, i + len - 1, j + len - 1) >= K)
						return {i, j};
				}
			}
			return {-1, -1};
		};

		// We will binary search on the side length
		ll L{1}, R = min(N, M);
		ll sideLength = -1;
		pair<int, int> topLeft{-1, -1};
		while (L <= R)
		{
			ll mid = ((L + R) >> 1);
			auto [x, y] = checkSubGrid(N, M, K, mid);
			if (x != -1 && y != -1)
				sideLength = mid, topLeft = {x, y}, R = mid - 1;
			else
				L = mid + 1;
		}
		cout << sideLength << endl;
		if (~sideLength)
			cout << topLeft.first << " " << topLeft.second;
	}
	return 0;
}