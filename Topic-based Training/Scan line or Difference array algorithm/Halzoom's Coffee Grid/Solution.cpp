#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
	int t = 1;
	// cin >> t;
	while (t--)
	{
		int n, m, k;
		cin >> n >> m >> k;
		vector<vector<ll>> vc(n + 1, vector<ll>(m + 1));
		for (int i{1}; i <= n; i++)
		{
			for (int j{1}; j <= m; j++)
				cin >> vc[i][j];
		}
		vector<vector<ll>> preSum(n + 1, vector<ll>(m + 1));
		for (int i{1}; i <= n; i++)
		{
			for (int j{1}; j <= m; j++)
				preSum[i][j] += preSum[i - 1][j] + preSum[i][j - 1] - preSum[i - 1][j - 1] + vc[i][j];
		}
		vector<vector<int>> diff(n + 2, vector<int>(m + 2, 0));
		for (int top = 1; top <= n; top++)
		{
			for (int bottom = top; bottom <= n; bottom++)
			{
				unordered_map<ll, vector<int>> sumPos;
				sumPos[0] = {0};
				ll currSum = 0;
				for (int col = 1; col <= m; col++)
				{
					ll colSum = preSum[bottom][col] - preSum[top - 1][col] - preSum[bottom][col - 1] + preSum[top - 1][col - 1];
					currSum += colSum;

					if (sumPos.count(currSum - k))
					{
						for (const int &left : sumPos[currSum - k])
						{
							int r1 = top, c1 = left + 1;
							int r2 = bottom, c2 = col;
							diff[r1][c1]++;
							diff[r1][c2 + 1]--;
							diff[r2 + 1][c1]--;
							diff[r2 + 1][c2 + 1]++;
						}
					}
					sumPos[currSum].push_back(col);
				}
			}
		}
		for (int i = 1; i <= n; i++)
		{
			for (int j = 1; j <= m; j++)
				diff[i][j] += diff[i - 1][j] + diff[i][j - 1] - diff[i - 1][j - 1];
		}
		for (int i{1}; i <= n; i++)
		{
			for (int j{1}; j <= m; j++)
			{
				if (!diff[i][j])
					vc[i][j] = 0;
				cout << vc[i][j] << " ";
			}
			cout << endl;
		}
	}
	return 0;
}