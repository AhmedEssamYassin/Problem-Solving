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
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
	int t = 1;
	ll N, M, K;
	cin >> t;
	while (t--)
	{
		cin >> N;
		vector<ll> p(N + 1);
		for (int i{1}; i <= N; i++)
			cin >> p[i];
		// Each connected component will have the same cycle length
		// This cycle length is the size of that connected component
		DSU dsu(N + 1);
		for (int i{1}; i <= N; i++)
			dsu.Union(i, p[i]);
		for (int i = 1; i <= N; i++)
			cout << dsu.size[dsu.Find(i)] << " ";
		cout << endl;
	}
	return 0;
}