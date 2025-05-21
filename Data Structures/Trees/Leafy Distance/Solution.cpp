#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

/*
A Tree is a connected graph without cycles.
Leaves are nodes with degree = 1 (connected to only one other node).
To find the best hiding place (node farthest from any exit),
we do a multi-source BFS starting from all leaves.
This ensures we measure the distance of each node from its closest leaf (exit).
Then, among all nodes, we choose the one with the maximum distance.
If there are multiple such nodes, we return the one with the least index.
*/

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
	int t;
	ll N;
	cin >> t;
	while (t--)
	{
		cin >> N;
		vector<vector<ll>> Tree(N + 1);
		vector<int> degree(N + 1, 0);

		for (int i = 0; i < N - 1; ++i)
		{
			ll u, v;
			cin >> u >> v;
			Tree[u].push_back(v);
			Tree[v].push_back(u);
			degree[u]++;
			degree[v]++;
		}

		queue<int> q;
		vector<int> dist(N + 1, -1);

		// Start multi-source BFS from all leaves (degree == 1)
		for (int i = 1; i <= N; ++i)
		{
			if (degree[i] == 1) // Leaf node
			{
				q.push(i);
				dist[i] = 0;
			}
		}

		while (!q.empty())
		{
			int u = q.front();
			q.pop();
			for (const int &v : Tree[u])
			{
				if (dist[v] == -1)
				{
					dist[v] = dist[u] + 1;
					q.push(v);
				}
			}
		}

		int maxDist = -1, ans = -1;

		// Choose the node with maximum distance (min index on tie)
		for (int i = 1; i <= N; ++i)
		{
			if (dist[i] > maxDist || (dist[i] == maxDist && i < ans))
			{
				maxDist = dist[i];
				ans = i;
			}
		}
		cout << ans << endl;
	}
	return 0;
}