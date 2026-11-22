#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

bool isOk(const vector<vector<pair<int, int>>> &adj, int n, long double mid, vector<int> &parent)
{
	const long double INF = 1e18;
	vector<long double> dp(n + 1, INF);
	parent.assign(n + 1, -1);
	dp[1] = 0;
	for (int u = 1; u < n; u++)
	{
		if (dp[u] >= INF / 2)
			continue;
		for (const auto &[v, w] : adj[u])
		{
			long double cand = dp[u] + (w - mid);
			if (cand < dp[v])
			{
				dp[v] = cand;
				parent[v] = u;
			}
		}
	}
	return dp[n] <= 0;
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
		int n, m;
		cin >> n >> m;
		vector<vector<pair<int, int>>> adj(n + 1);
		for (int i{}; i < m; i++)
		{
			int u, v, w;
			cin >> u >> v >> w;
			adj[u].push_back({v, w});
		}
		long double L = 0, R = 100;
		vector<int> parent, bestParent;
		isOk(adj, n, R, bestParent);
		for (int i{}; i < 80; i++)
		{
			long double mid = (L + R) / 2;
			if (isOk(adj, n, mid, parent))
			{
				R = mid;
				bestParent = parent;
			}
			else
				L = mid;
		}
		vector<int> path;
		for (int cur = n; cur != -1; cur = bestParent[cur])
			path.push_back(cur);
		reverse(path.begin(), path.end());
		cout << (int)path.size() - 1 << endl;
		for (int i{}; i < (int)path.size(); i++)
			cout << path[i] << " ";
	}
	return 0;
}
