#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct Edge
{
	int u, v, id;
	ll cost;
	Edge(int u, int v, int id, ll cost) : u(u), v(v), id(id), cost(cost) {}
	bool operator<(const Edge &e) const { return cost > e.cost; }
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
		ll s;
		cin >> n >> m >> s;
		vector<Edge> edges;
		edges.reserve(m);
		for (int i = 1; i <= m; i++)
		{
			int u, v;
			ll c;
			cin >> u >> v >> c;
			edges.emplace_back(u, v, i, c);
		}
		sort(edges.begin(), edges.end());
		DSU disjointUnion(n);
		vector<bool> inMST(m + 1, false);
		for (const auto &[u, v, id, cost] : edges)
		{
			if (!disjointUnion.isSameComponent(u, v))
			{
				disjointUnion.Union(u, v);
				inMST[id] = true;
			}
		}
		vector<int> deleted;
		ll used = 0;
		for (int i = m - 1; i >= 0; i--)
		{
			if (!inMST[edges[i].id] && used + edges[i].cost <= s)
			{
				used += edges[i].cost;
				deleted.push_back(edges[i].id);
			}
		}
		sort(deleted.begin(), deleted.end());
		cout << deleted.size() << endl;
		for (int i{}; i < (int)deleted.size(); i++)
			cout << deleted[i] << " ";
	}
	return 0;
}
