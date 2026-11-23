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

struct Query
{
	int l, r, id;
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
		vector<pair<int, int>> edges(m);
		for (int i{}; i < m; i++)
			cin >> edges[i].first >> edges[i].second;

		int k;
		cin >> k;
		int B = max(1, (int)sqrt(m));
		int numBlocks = (m + B - 1) / B;
		vector<vector<Query>> buckets(numBlocks);
		vector<int> ans(k);
		DSU disjointUnion(n);

		for (int i{}; i < k; i++)
		{
			int l, r;
			cin >> l >> r;
			l--, r--;
			if (l / B == r / B)
			{
				disjointUnion.persist();
				for (int j = l; j <= r; j++)
					disjointUnion.Union(edges[j].first, edges[j].second);
				ans[i] = disjointUnion.components;
				disjointUnion.rollback();
			}
			else
				buckets[l / B].push_back({l, r, i});
		}

		for (int b{}; b < numBlocks; b++)
		{
			if (buckets[b].empty())
				continue;
			sort(buckets[b].begin(), buckets[b].end(), [](const Query &a, const Query &c)
			     { return a.r < c.r; });
			int border = (b + 1) * B;
			int curR = border - 1;
			disjointUnion.persist();
			for (const auto &[l, r, id] : buckets[b])
			{
				while (curR < r)
				{
					curR++;
					disjointUnion.Union(edges[curR].first, edges[curR].second);
				}
				disjointUnion.persist();
				for (int j = border - 1; j >= l; j--)
					disjointUnion.Union(edges[j].first, edges[j].second);
				ans[id] = disjointUnion.components;
				disjointUnion.rollback();
			}
			disjointUnion.rollback();
		}

		for (int i{}; i < k; i++)
			cout << ans[i] << endl;
	}
	return 0;
}
