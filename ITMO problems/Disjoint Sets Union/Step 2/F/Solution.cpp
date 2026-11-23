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
	int N, components;
	// The leader should be the parent of itself
	DSU(int _N)
	{
		N = _N;
		parent.resize(N + 1);
		size.resize(N + 1);
		reset();
	}
	void reset()
	{
		components = N;
		iota(parent.begin(), parent.end(), 0);
		fill(size.begin(), size.end(), 1); // Each component is of size 1 initially
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
		components--;
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
		ll best = LLONG_MAX;
		for (int i{}; i + n - 1 <= m; i++)
		{
			if (i > 0 && edges[i].cost == edges[i - 1].cost)
				continue;
			disjointUnion.reset();
			for (int j = i; j < m; j++)
			{
				if (edges[j].cost - edges[i].cost >= best)
					break;
				disjointUnion.Union(edges[j].u, edges[j].v);
				if (disjointUnion.components == 1)
				{
					best = min(best, edges[j].cost - edges[i].cost);
					break;
				}
			}
		}
		if (best == LLONG_MAX)
			cout << "NO";
		else
			cout << "YES" << endl
			     << best;
	}
	return 0;
}
