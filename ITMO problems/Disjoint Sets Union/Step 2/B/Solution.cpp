#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct DSU
{
	vector<int> parent, size, nextFree; // Representative
	int N;
	// The leader should be the parent of itself
	DSU(int _N)
	{
		N = _N;
		parent.resize(N + 1);
		size.resize(N + 1, 1); // Each component is of size 1 initially
		nextFree.resize(N + 1);
		iota(parent.begin(), parent.end(), 0);
		iota(nextFree.begin(), nextFree.end(), 0);
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
	int park(int pos)
	{
		int slot = nextFree[Find(pos)];
		int nxt = (slot == N ? 1 : slot + 1);
		int target = nextFree[Find(nxt)];
		Union(slot, nxt);
		nextFree[Find(slot)] = target;
		return slot;
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
		int n;
		cin >> n;
		DSU disjointUnion(n);
		for (int i{}; i < n; i++)
		{
			int p;
			cin >> p;
			cout << disjointUnion.park(p) << " ";
		}
	}
	return 0;
}
