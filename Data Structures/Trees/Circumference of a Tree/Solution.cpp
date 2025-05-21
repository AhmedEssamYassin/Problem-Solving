#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

/*
Unlike Graphs, a Tree doesn't need a `visited` array for DFS because it has NO cycles.
In DFS on a Tree, we simply avoid revisiting the parent node (previous node).
*/

// Diameter = number of edges on the longest path between two nodes.
void findFarthestNode(vector<vector<ll>> &Tree, int u, int parent, int dist, int &maxDist, int &farthestNode)
{
	if (dist >= maxDist)
	{
		maxDist = dist;
		farthestNode = u;
	}
	for (const ll &v : Tree[u])
	{
		if (v != parent)
			findFarthestNode(Tree, v, u, dist + 1, maxDist, farthestNode);
	}
}

int treeDiameter(vector<vector<ll>> &Tree, int root)
{
	int maxDist = 0;
	int x; // Used to track farthest node.

	// DFS from a random node and then see farthest node `x` from it
	findFarthestNode(Tree, root, -1, 0, maxDist, x);

	// DFS from `x` and check the farthest node from it
	findFarthestNode(Tree, x, -1, 0, maxDist, x);

	return maxDist;
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
		cin >> N;
		vector<vector<ll>> Tree(N + 1);
		int anyNode = 1;
		for (int i{}; i < N - 1; i++)
		{
			ll u, v;
			cin >> u >> v;
			anyNode = u;
			Tree[u].push_back(v);
			Tree[v].push_back(u);
		}
		ll root = 1;
		// If the tree is not rooted
		root = anyNode;
		ll diameter = treeDiameter(Tree, root);
		ll PI = 3;
		cout << PI * diameter;
	}
	return 0;
}