#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct DSU
{
	vector<int> parent, size, maximum; // Representative
	int N;
	// The leader should be the parent of itself
	DSU(int _N)
	{
		N = _N;
		parent.resize(N + 1);
		size.resize(N + 1, 1); // Each component is of size 1 initially
		maximum.resize(N + 1);
		iota(parent.begin(), parent.end(), 0);
		iota(maximum.begin(), maximum.end(), 0);
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
		DSU disjointUnion(n + 1);
		while (m--)
		{
			char type;
			int x;
			cin >> type >> x;
			if (type == '-')
				disjointUnion.Union(x, x + 1);
			else
			{
				int ans = disjointUnion.maximum[disjointUnion.Find(x)];
				cout << (ans > n ? -1 : ans) << endl;
			}
		}
	}
	return 0;
}
