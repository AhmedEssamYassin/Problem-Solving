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

void addEdge(vector<vector<pair<int, int>>> &tree, const vector<int> &prefQuery, int node, int l, int r, int ql, int qr, pair<int, int> e)
{
	if (ql > r || qr < l || prefQuery[r + 1] - prefQuery[l] == 0)
		return;
	if (ql <= l && r <= qr)
	{
		tree[node].push_back(e);
		return;
	}
	int mid = (l + r) / 2;
	addEdge(tree, prefQuery, 2 * node, l, mid, ql, qr, e);
	addEdge(tree, prefQuery, 2 * node + 1, mid + 1, r, ql, qr, e);
}

void dfs(const vector<vector<pair<int, int>>> &tree, const vector<int> &prefQuery, DSU &dsu, const vector<bool> &isQuery, vector<int> &ans, int node, int l, int r)
{
	if (prefQuery[r + 1] - prefQuery[l] == 0)
		return;
	dsu.persist();
	for (const auto &[u, v] : tree[node])
		dsu.Union(u, v);
	if (l == r)
	{
		if (isQuery[l])
			ans[l] = dsu.components;
	}
	else
	{
		int mid = (l + r) / 2;
		dfs(tree, prefQuery, dsu, isQuery, ans, 2 * node, l, mid);
		dfs(tree, prefQuery, dsu, isQuery, ans, 2 * node + 1, mid + 1, r);
	}
	dsu.rollback();
}

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
		if (m == 0)
			return 0;

		vector<bool> isQuery(m, false);
		vector<int> prefQuery(m + 1, 0);
		struct Interval
		{
			int l, r, u, v;
		};
		vector<Interval> intervals;
		unordered_map<uint64_t, int> active;
		active.reserve(m);

		for (int i{}; i < m; i++)
		{
			char type;
			cin >> type;
			if (type == '?')
				isQuery[i] = true;
			else
			{
				int u, v;
				cin >> u >> v;
				if (u > v)
					swap(u, v);
				uint64_t key = ((uint64_t)u << 32) | (uint32_t)v;
				if (type == '+')
					active[key] = i;
				else
				{
					auto it = active.find(key);
					if (it->second <= i - 1)
						intervals.push_back({it->second, i - 1, u, v});
					active.erase(it);
				}
			}
			prefQuery[i + 1] = prefQuery[i] + isQuery[i];
		}

		for (const auto &[key, start] : active)
		{
			int u = (int)(key >> 32);
			int v = (int)(uint32_t)key;
			if (start <= m - 1)
				intervals.push_back({start, m - 1, u, v});
		}

		vector<vector<pair<int, int>>> tree(4 * m);
		for (const auto &iv : intervals)
			addEdge(tree, prefQuery, 1, 0, m - 1, iv.l, iv.r, {iv.u, iv.v});

		DSU disjointUnion(n);
		vector<int> ans(m, 0);
		dfs(tree, prefQuery, disjointUnion, isQuery, ans, 1, 0, m - 1);

		for (int i{}; i < m; i++)
		{
			if (isQuery[i])
				cout << ans[i] << endl;
		}
	}
	return 0;
}
