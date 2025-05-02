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
	ll N, a, b;
	cin >> t;
	while (t--)
	{
		cin >> N;
		DSU disjointUnion(2 * N); // 2N nodes
		vector<ll> freq(N + 1, 0);
		for (int i{}; i < N; i++)
		{
			cin >> a >> b;
			freq[a]++;
			freq[b]++;
			disjointUnion.Union(a, b + N); // In one set
			disjointUnion.Union(a + N, b); // In the other set
		}
		bool flag = true;
		for (int i{1}; i <= N; i++)
		{
			if (freq[i] > 2)
			{
				flag = false;
				break;
			}
		}
		for (int i{1}; i <= N; i++)
		{
			if (disjointUnion.isSameComponent(i, i + N))
			{
				flag = false;
				break;
			}
		}
		if (flag)
			cout << "YES\n";
		else
			cout << "NO\n";
	}
	return 0;
}