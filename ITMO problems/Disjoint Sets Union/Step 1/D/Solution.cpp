#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct DSU
{
	vector<ll> parent, size; // Representative
	int N;
	// The leader should be the parent of itself
	DSU(int _N)
	{
		N = _N;
		parent.resize(N + 1);
		size.resize(N + 1, 1); // Each component if of size 1 initially
		iota(parent.begin(), parent.end(), 0);
	}
	ll Find(ll node)
	{
		if (parent[node] == node)
			return node;

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
	ll N, M, K;
	// cin >> t;
	while (t--)
	{
		cin >> N >> M >> K;
		ll u, v;
		while (M--)
			cin >> u >> v; // Redundant
		string query;
		DSU disjointUnion(N);
		vector<array<ll, 3>> Q(K);
		for (int i{}; i < K; i++)
		{
			cin >> query >> u >> v;
			if (query == "ask")
				Q[i] = {0, u, v};
			else
				Q[i] = {1, u, v};
		}
		stack<string> ans;
		for (int i = K - 1; i >= 0; i--)
		{
			const auto &[query, u, v] = Q[i];
			if (query == 0)
			{
				if (disjointUnion.isSameComponent(u, v))
					ans.push("YES");
				else
					ans.push("NO");
			}
			else
				disjointUnion.Union(u, v);
		}
		while (!ans.empty())
			cout << ans.top() << endl, ans.pop();
	}
	return 0;
}