#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct DSU
{
	vector<ll> parent, size, exp; // Representative
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
	ll Find(ll node)
	{
		if (parent[node] == node)
			return node;
		ll up = Find(parent[node]);
		if (parent[node] != up)
		{
			exp[node] += exp[parent[node]];
			parent[node] = up;
		}
		return parent[node] = Find(parent[node]); // Path compression
	}
	void Union(ll u, ll v)
	{
		ll rep1 = Find(u);
		ll rep2 = Find(v);
		if (rep1 == rep2)
			return;
		// Small-to-large technique
		if (size[rep1] > size[rep2])
			swap(rep1, rep2); // representative of smaller set comes first
		parent[rep1] = rep2;
		size[rep2] += size[rep1];
		exp[rep1] -= exp[rep2];
	}
	void add(ll node, ll val)
	{
		ll rep = Find(node);
		exp[rep] += val;
	}
	bool isSameComponent(ll u, ll v)
	{
		return (Find(u) == Find(v));
	}
	set<ll> findConnectedComponents()
	{
		set<ll> st;
		// Traverse all vertices
		for (int i = 1; i <= N; i++)
			st.insert(Find(parent[i]));

		return st;
	}
	ll query(ll node)
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
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
	int t = 1;
	ll N, M;
	// cin >> t;
	while (t--)
	{
		cin >> N >> M;
		string query;
		ll u, v;
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