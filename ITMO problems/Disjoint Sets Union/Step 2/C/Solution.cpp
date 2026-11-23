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
		int n, q;
		cin >> n >> q;
		DSU teamDSU(n), nextDSU(n);
		while (q--)
		{
			int type, x, y;
			cin >> type >> x >> y;
			if (type == 1)
				teamDSU.Union(x, y);
			else if (type == 2)
			{
				int cur = nextDSU.maximum[nextDSU.Find(x)];
				while (cur < y)
				{
					teamDSU.Union(cur, cur + 1);
					nextDSU.Union(cur, cur + 1);
					cur = nextDSU.maximum[nextDSU.Find(cur)];
				}
			}
			else
				cout << (teamDSU.isSameComponent(x, y) ? "YES" : "NO") << endl;
		}
	}
	return 0;
}
