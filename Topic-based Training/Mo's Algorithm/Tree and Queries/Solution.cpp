#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

/*
Fenwick Tree: Binary Indexed Tree (BIT)
0-based version
Works for invertible operations (such as Summation (+) and XOR (^))
*/
class FenwickTree
{
private:
	int N;
	vector<ll> BIT;

public:
	FenwickTree(int sz)
	{
		N = sz;
		BIT.assign(N + 1, 0);
	}
	void update(int pos, const int &delta)
	{
		for (int i = pos; i < N; i |= (i + 1))
			BIT[i] += delta;
	}

	ll query(int pos)
	{
		ll sum = 0;
		for (int i = pos; i >= 0; i = (i & (i + 1)) - 1)
			sum += BIT[i];

		return sum;
	}

	ll rangeQuery(int L, int R)
	{
		if (L > R)
			return 0; // Neutral value
		if (L == 0)
			return query(R);
		return (query(R) - query(L - 1));
	}
};

// ─── Hilbert Curve Ordering ───────────────────────────────────────────────────
uint64_t hilbertOrder(uint64_t x, uint64_t y)
{
	const uint64_t logN = __lg(max(x, y) * 2 + 1) | 1;
	const uint64_t maxN = (1ULL << logN) - 1;
	uint64_t res = 0;
	for (uint64_t s = 1ULL << (logN - 1); s; s >>= 1)
	{
		bool rx = x & s, ry = y & s;
		res = (res << 2) | (rx ? (ry ? 2 : 1) : (ry ? 3 : 0));
		if (!rx)
		{
			if (ry)
				x ^= maxN, y ^= maxN;
			swap(x, y);
		}
	}
	return res;
}

struct Query
{
	int l, r, k, id;
	uint64_t hilbert;

	Query(int l, int r, int k, int id)
		: l(l), r(r), k(k), id(id), hilbert(hilbertOrder(l, r)) {}

	bool operator<(const Query &o) const { return hilbert < o.hilbert; }
};

template <typename AddFn, typename RemoveFn, typename AnswerFn>
vector<ll> runMo(vector<Query> queries,
				 AddFn add, RemoveFn remove, AnswerFn getAnswer)
{
	sort(queries.begin(), queries.end()); // Hilbert order, O(m log m) comparisons, O(1) each

	vector<ll> ans(queries.size());
	int curL = 0, curR = -1;

	for (const auto &[l, r, k, id, h] : queries)
	{
		while (curL > l)
			add(--curL);
		while (curR < r)
			add(++curR);
		while (curL < l)
			remove(curL++);
		while (curR > r)
			remove(curR--);
		ans[id] = getAnswer(k);
	}
	return ans;
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
		vector<int> c(n + 1);
		for (int i{1}; i <= n; i++)
			cin >> c[i];
		vector<vector<int>> Tree(n + 1);
		for (int i{}; i < n - 1; i++)
		{
			int u, v;
			cin >> u >> v;
			// Tree edges are indirected
			Tree[u].push_back(v);
			Tree[v].push_back(u);
		}
		int root = 1;
		int currTime = 0;
		vector<int> in(n + 1), out(n + 1);
		function<void(int, int)> eulerTour = [&](int u, int prev) -> void
		{
			in[u] = currTime++;
			for (const ll &v : Tree[u])
			{
				if (v != prev)
					eulerTour(v, u);
			}
			out[u] = currTime - 1;
		};
		eulerTour(root, -1); // root is stated to be 1
		vector<int> nodes(n);
		for (int u = 1; u <= n; u++)
			nodes[in[u]] = c[u];

		vector<Query> queries;
		queries.reserve(m);
		for (int i = 0; i < m; i++)
		{
			int v, k;
			cin >> v >> k;
			int l = in[v], r = out[v];
			queries.emplace_back(l, r, k, i); // hilbert computed once
		}
		vector<int> freq(1e5);
		FenwickTree BIT(n + 1);
		auto add = [&](int idx)
		{
			int &f = freq[nodes[idx]];
			if (f > 0)
				BIT.update(f, -1);
			f++;
			BIT.update(f, +1);
		};

		auto remove = [&](int idx)
		{
			int &f = freq[nodes[idx]];
			if (f > 0)
				BIT.update(f, -1);
			f--;
			if (f > 0)
				BIT.update(f, +1);
		};
		auto getAnswer = [&](int k)
		{ return BIT.rangeQuery(k, n); };

		vector<ll> ans = runMo(queries, add, remove, getAnswer);
		for (const auto &x : ans)
			cout << x << endl;
	}
	return 0;
}