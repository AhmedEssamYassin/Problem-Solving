#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

bool isOk(const vector<vector<pair<int, ll>>> &adj, int n, int d, ll maxW, vector<int> &parent)
{
	vector<int> dist(n + 1, -1);
	parent.assign(n + 1, -1);
	queue<int> q;
	dist[1] = 0;
	q.push(1);
	while (!q.empty())
	{
		int u = q.front();
		q.pop();
		if (dist[u] == d)
			continue;
		for (const auto &[v, w] : adj[u])
		{
			if (w <= maxW && dist[v] == -1)
			{
				dist[v] = dist[u] + 1;
				parent[v] = u;
				q.push(v);
			}
		}
	}
	return (dist[n] != -1 && dist[n] <= d);
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
		int n, m, d;
		cin >> n >> m >> d;
		vector<vector<pair<int, ll>>> adj(n + 1);
		ll maxEdge = 0;
		for (int i{}; i < m; i++)
		{
			int u, v;
			ll c;
			cin >> u >> v >> c;
			adj[u].push_back({v, c});
			maxEdge = max(maxEdge, c);
		}
		ll L = 0, R = maxEdge, ans = -1;
		vector<int> parent, bestParent;
		while (L <= R)
		{
			ll mid = L + (R - L) / 2;
			if (isOk(adj, n, d, mid, parent))
			{
				ans = mid;
				bestParent = parent;
				R = mid - 1;
			}
			else
				L = mid + 1;
		}
		if (ans == -1)
		{
			cout << -1;
			return 0;
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
