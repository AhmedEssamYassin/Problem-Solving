#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct Edge
{
	int u, v;
	ll cost;
	Edge(int u, int v, ll cost) : u(u), v(v), cost(cost) {}
	bool operator<(const Edge &e) const { return cost < e.cost; }
};

struct DSU
{
	vector<int> parent, size; // Representative
	int N;
	// The leader should be the parent of itself
	DSU(int _N)
	{
		N = _N;
		parent.resize(N + 1);
		size.resize(N + 1, 1); // Each component is of size 1 initially
		iota(parent.begin(), parent.end(), 0);
	}
	int Find(int node)
	{
		if (parent[node] == node)
			return node;

		return parent[node] = Find(parent[node]); // Path compression
	}
	void Union(int u, int v)
	{
		int rep1 = Find(u);
		int rep2 = Find(v);
		if (rep1 == rep2)
			return;
		// Small-to-large technique
		if (size[rep1] > size[rep2])
			swap(rep1, rep2); // representative of smaller set comes first
		parent[rep1] = rep2;
		size[rep2] += size[rep1];
	}
	bool isSameComponent(int u, int v)
	{
		return (Find(u) == Find(v));
	}
};

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
		vector<Edge> edges;
		edges.reserve(m);
		for (int i{}; i < m; i++)
		{
			int u, v;
			ll w;
			cin >> u >> v >> w;
			edges.emplace_back(u, v, w);
		}
		sort(edges.begin(), edges.end());
		DSU disjointUnion(n);
		ll mstCost = 0;
		for (const auto &[u, v, cost] : edges)
		{
			if (!disjointUnion.isSameComponent(u, v))
			{
				disjointUnion.Union(u, v);
				mstCost += cost;
			}
		}
		cout << mstCost;
	}
	return 0;
}
