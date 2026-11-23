#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct DSU
{
	vector<int> parent, size; // Representative
	vector<pair<int, int>> history;
	vector<int> checkpoints;
	int N, components;
	// The leader should be the parent of itself
	DSU(int _N)
	{
		N = _N;
		components = _N;
		parent.resize(N + 1);
		size.resize(N + 1, 1); // Each component is of size 1 initially
		iota(parent.begin(), parent.end(), 0);
	}
	int Find(int node)
	{
		if (parent[node] == node)
			return node;

		return Find(parent[node]); // No path compression to support rollback
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
		history.emplace_back(rep1, rep2);
		components--;
	}
	void persist()
	{
		checkpoints.push_back((int)history.size());
	}
	void rollback()
	{
		int target = checkpoints.back();
		checkpoints.pop_back();
		while ((int)history.size() > target)
		{
			auto [u, v] = history.back();
			history.pop_back();
			parent[u] = u;
			size[v] -= size[u];
			components++;
		}
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
			string op;
			cin >> op;
			if (op == "union")
			{
				int u, v;
				cin >> u >> v;
				disjointUnion.Union(u, v);
				cout << disjointUnion.components << endl;
			}
			else if (op == "persist")
				disjointUnion.persist();
			else
			{
				disjointUnion.rollback();
				cout << disjointUnion.components << endl;
			}
		}
	}
	return 0;
}
