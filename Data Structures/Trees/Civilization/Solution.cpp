#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct DSU
{
	vector<ll> parent, size; // Representative
	int N;
	// The leader should be the parent of itself
	DSU(int _N)
	{
		N = _N;
		parent.resize(N + 1);
		size.resize(N + 1, 1); // Each component if of size 1 initially
		iota(parent.begin(), parent.end(), 0);
	}
	ll Find(ll node)
	{
		if (parent[node] == node)
			return node;

		return parent[node] = Find(parent[node]); // Path compression
	}
	void Union(ll u, ll v)
	{
		ll rep1 = Find(u);
		ll rep2 = Find(v);
		if (rep1 == rep2)
			return;
		// Small-to-large technique
		if (size[rep1] > size[rep2])
			swap(rep1, rep2); // representative of smaller set comes first
		parent[rep1] = rep2;
		size[rep2] += size[rep1];
	}
	bool isSameComponent(ll u, ll v)
	{
		return (Find(u) == Find(v));
	}
	set<ll> findConnectedComponents()
	{
		set<ll> st;
		// Traverse all vertices
		for (int i = 1; i <= N; i++)
			st.insert(Find(parent[i]));

		return st;
	}
};

/*
Unlike Graphs, a Tree doesn't need a `visited` array for DFS because it has NO cycles.
In DFS on a Tree, we simply avoid revisiting the parent node (previous node).
*/

// Diameter = number of edges on the longest path between two nodes.
vector<bool> vis;
void findFarthestNode(vector<vector<ll>> &Tree, int u, int parent, int dist, int &maxDist, int &farthestNode)
{
	if (dist >= maxDist)
	{
		maxDist = dist;
		farthestNode = u;
	}
	vis[u] = true;
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
	ll N, M, Q;
	// cin >> t;
	while (t--)
	{
		/*
		The center of a tree is the node (or one of the two nodes) that lies in the middle of the longest path (diameter).
		When you connect two centers, you are balancing the depth added to both sides.
		Connecting from extremes (diameter ends) would increase the new diameter more than necessary.
		*/
		cin >> N >> M >> Q;
		vector<vector<ll>> Tree(N + 1);
		DSU dsu(N + 1);
		vector<ll> diameter(N + 1);
		for (int i{}; i < M; i++)
		{
			ll u, v;
			cin >> u >> v;
			dsu.Union(u, v);
			Tree[u].push_back(v);
			Tree[v].push_back(u);
		}
		vis.assign(N + 1, false);
		for (int u = 1; u <= N; u++)
		{
			if (!vis[u])
				diameter[dsu.Find(u)] = treeDiameter(Tree, u);
		}
		while (Q--)
		{
			ll query, x, y;
			cin >> query;
			if (query == 1)
			{
				cin >> x;
				cout << diameter[dsu.Find(x)] << endl;
			}
			else
			{
				cin >> x >> y;
				if (dsu.Find(x) == dsu.Find(y))
					continue;
				ll d1 = diameter[dsu.Find(x)];
				ll d2 = diameter[dsu.Find(y)];
				ll D = max(d1, d2);
				ll r1 = (d1 + 1) / 2;
				ll r2 = (d2 + 1) / 2;
				ll newDiameter = max(D, r1 + r2 + 1);
				dsu.Union(x, y);
				diameter[dsu.Find(x)] = newDiameter;
			}
		}
	}
	return 0;
}