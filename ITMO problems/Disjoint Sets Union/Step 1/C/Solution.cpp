#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct DSU
{
	vector<int> parent, size; // Representative
	vector<ll> exp;
	int N;
	// The leader should be the parent of itself
	DSU(int _N)
	{
		N = _N;
		parent.resize(N + 1);
		size.resize(N + 1, 1); // Each component if of size 1 initially
		exp.resize(N + 1, 0);
		iota(parent.begin(), parent.end(), 0);
	}
	int Find(int node)
	{
		if (parent[node] == node)
			return node;
		int up = Find(parent[node]);
		if (parent[node] != up)
		{
			exp[node] += exp[parent[node]];
			parent[node] = up;
		}
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
		exp[rep1] -= exp[rep2];
	}
	void add(int node, ll val)
	{
		int rep = Find(node);
		exp[rep] += val;
	}
	ll query(int node)
	{
		if (parent[node] == node)
			return exp[node];
		return exp[node] + query(parent[node]);
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
		int N, M;
		cin >> N >> M;
		string query;
		int u, v;
		DSU disjointUnion(N);
		while (M--)
		{
			cin >> query;
			if (query == "add")
			{
				cin >> u >> v;
				disjointUnion.add(u, v);
			}

			else if (query == "join")
			{
				cin >> u >> v;
				disjointUnion.Union(u, v);
			}

			else
			{
				cin >> v;
				ll ans = disjointUnion.query(v);
				cout << ans << endl;
			}
		}
	}
	return 0;
}