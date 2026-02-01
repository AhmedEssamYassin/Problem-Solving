#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct DSU
{
	vector<int> parent, size; // Representative
	int N;
	// The leader should be the parent of itself
	DSU(int _N)
	{
		N = _N;
		parent.resize(N + 1);
		size.resize(N + 1, 1); // Each component if of size 1 initially
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
	set<int> findConnectedComponents()
	{
		set<int> st;
		// Traverse all vertices
		for (int i = 1; i <= N; i++)
			st.insert(Find(parent[i]));

		return st;
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
	ll N, M;
	// cin >> t;
	while (t--)
	{
		cin >> N >> M;
		vector<ll> cost(N + 1); // {cost, order}
		for (int i{1}; i <= N; i++)
			cin >> cost[i];
		DSU dsu(N + 1);
		map<ll, set<ll>> Components; //{representative, costs}
		for (int i{}; i < M; i++)
		{
			ll x, y;
			cin >> x >> y;
			if (not dsu.isSameComponent(x, y))
				dsu.Union(x, y);
		}
		// Calculating cost of all components with minimal cost of each group
		ll totalCost{};
		for (int i = 1; i <= N; i++)
			Components[dsu.Find(i)].insert(cost[i]);

		for (const auto &[rep, st] : Components)
			totalCost += *st.begin();

		cout << totalCost;
	}
	return 0;
}