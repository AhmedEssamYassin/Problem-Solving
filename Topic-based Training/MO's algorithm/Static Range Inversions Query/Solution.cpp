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
		if (L > R || L <= 0)
			return 0; // Neutral value
		return (query(R) - query(L - 1));
	}
};

class CoordinateCompression
{
private:
	vector<ll> init;
	void compress(vector<ll> &vec)
	{
		sort(vec.begin(), vec.end());
		vec.erase(unique(vec.begin(), vec.end()), vec.end());
	}

public:
	CoordinateCompression() {}
	CoordinateCompression(const vector<ll> &vec)
	{
		init = vec;
		compress(init);
	}
	int index(ll val) { return lower_bound(init.begin(), init.end(), val) - init.begin(); }
	ll initialValue(int idx) { return init[idx]; }
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
	int l, r, id;
	uint64_t hilbert;

	Query(int l, int r, int id)
		: l(l), r(r), id(id), hilbert(hilbertOrder(l, r)) {}

	bool operator<(const Query &o) const { return hilbert < o.hilbert; }
};

template <typename AddLeftFn, typename AddRightFn, typename RemoveLeftFn, typename RemoveRightFn, typename AnswerFn>
vector<ll> runMo(vector<Query> queries,
				 AddLeftFn addLeft, AddRightFn addRight,
				 RemoveLeftFn removeLeft, RemoveRightFn removeRight,
				 AnswerFn getAnswer)
{
	sort(queries.begin(), queries.end()); // Hilbert order, O(m log m) comparisons, O(1) each

	vector<ll> ans(queries.size());
	int curL = 0, curR = -1;

	for (const auto &[l, r, id, h] : queries)
	{
		while (curL > l)
			addLeft(--curL);
		while (curR < r)
			addRight(++curR);
		while (curL < l)
			removeLeft(curL++);
		while (curR > r)
			removeRight(curR--);
		ans[id] = getAnswer();
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
		int n, q;
		cin >> n >> q;
		vector<ll> vc(n);
		for (int i{}; i < n; i++)
			cin >> vc[i];

		vector<Query> queries;
		queries.reserve(q);
		for (int i = 0; i < q; i++)
		{
			int l, r;
			cin >> l >> r;
			r--;
			queries.emplace_back(l, r, i); // hilbert computed once
		}

		CoordinateCompression comp(vc);
		for (int i = 0; i < n; i++)
			vc[i] = comp.index(vc[i]);
		FenwickTree BIT(n + 1);
		ll inv{};
		auto addRight = [&](int idx)
		{
			inv += BIT.rangeQuery(vc[idx] + 1, n);
			BIT.update(vc[idx], +1);
		};
		auto addLeft = [&](int idx)
		{
			inv += BIT.query(vc[idx] - 1);
			BIT.update(vc[idx], +1);
		};

		auto removeRight = [&](int idx)
		{
			inv -= BIT.rangeQuery(vc[idx] + 1, n);
			BIT.update(vc[idx], -1);
		};
		auto removeLeft = [&](int idx)
		{
			inv -= BIT.query(vc[idx] - 1);
			BIT.update(vc[idx], -1);
		};
		auto getAnswer = [&]()
		{ return inv; };

		vector<ll> ans = runMo(queries, addLeft, addRight, removeLeft, removeRight, getAnswer);
		for (const auto &x : ans)
			cout << x << endl;
	}
	return 0;
}