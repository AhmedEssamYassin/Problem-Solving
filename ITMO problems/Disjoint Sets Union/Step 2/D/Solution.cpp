#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct DSU
{
	vector<int> parent, size, depth; // Representative
	int N;
	// The leader should be the parent of itself
	DSU(int _N)
	{
		N = _N;
		parent.resize(N + 1);
		size.resize(N + 1, 1); // Each component is of size 1 initially
		depth.resize(N + 1, 0);
		iota(parent.begin(), parent.end(), 0);
	}
	int Find(int node)
	{
		if (parent[node] == node)
			return node;
		int up = Find(parent[node]);
		if (parent[node] != up)
		{
			depth[node] += depth[parent[node]];
			parent[node] = up;
		}
		return parent[node]; // Path compression
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
		depth[rep1] -= depth[rep2];
	}
	void add(int node, int val)
	{
		int rep = Find(node);
		depth[rep] += val;
	}
	int query(int node)
	{
		int root = Find(node);
		return (node == root ? depth[root] : depth[node] + depth[root]);
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
		DSU disjointUnion(n);
		while (m--)
		{
			int type;
			cin >> type;
			if (type == 1)
			{
				int a, b;
				cin >> a >> b;
				disjointUnion.add(a, 1);
				disjointUnion.Union(a, b);
			}
			else
			{
				int c;
				cin >> c;
				cout << disjointUnion.query(c) << endl;
			}
		}
	}
	return 0;
}
