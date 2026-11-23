#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct DSU
{
	vector<int> parent, size, maximum, minimum; // Representative
	int N;
	// The leader should be the parent of itself
	DSU(int _N)
	{
		N = _N;
		parent.resize(N + 1);
		maximum.resize(N + 1);
		minimum.resize(N + 1);
		size.resize(N + 1, 1); // Each component if of size 1 initially
		iota(parent.begin(), parent.end(), 0);
		iota(maximum.begin(), maximum.end(), 0);
		iota(minimum.begin(), minimum.end(), 0);
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
		maximum[rep2] = max(maximum[rep1], maximum[rep2]);
		minimum[rep2] = min(minimum[rep1], minimum[rep2]);
	}
	tuple<int, int, int> query(int node)
	{
		int rep = Find(node);
		return {minimum[rep], maximum[rep], size[rep]};
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
			if (query == "union")
			{
				cin >> u >> v;
				disjointUnion.Union(u, v);
			}
			else
			{
				cin >> v;
				tuple<int, int, int> ans = disjointUnion.query(v);
				cout << get<0>(ans) << " " << get<1>(ans) << " " << get<2>(ans) << endl;
			}
		}
	}
	return 0;
}