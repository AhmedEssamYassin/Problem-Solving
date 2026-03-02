#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

#include <ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;

struct custom_hash
{
	static uint64_t splitmix64(uint64_t x)
	{
		x += 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}
	size_t operator()(uint64_t x) const
	{
		static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
		return splitmix64(x + FIXED_RANDOM);
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
	int l, r, id;
	uint64_t hilbert;

	Query(int l, int r, int id)
		: l(l), r(r), id(id), hilbert(hilbertOrder(l, r)) {}

	bool operator<(const Query &o) const { return hilbert < o.hilbert; }
};

template <typename AddFn, typename RemoveFn, typename AnswerFn>
vector<ll> runMo(vector<Query> queries,
				 AddFn add, RemoveFn remove, AnswerFn getAnswer)
{
	sort(queries.begin(), queries.end()); // Hilbert order, O(m log m) comparisons, O(1) each

	vector<ll> ans(queries.size());
	int curL = 0, curR = -1;

	for (const auto &[l, r, id, h] : queries)
	{
		while (curL > l)
			add(--curL);
		while (curR < r)
			add(++curR);
		while (curL < l)
			remove(curL++);
		while (curR > r)
			remove(curR--);
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
		int n, m;
		cin >> n >> m;
		vector<ll> vc(n + 1);
		for (int i{1}; i <= n; i++)
			cin >> vc[i];

		vector<Query> queries;
		queries.reserve(m);
		for (int i = 0; i < m; i++)
		{
			int l, r;
			cin >> l >> r;
			queries.emplace_back(l, r, i); // hilbert computed once
		}

		ll currAns{};
		gp_hash_table<ll, ll, custom_hash> freq;
		auto add = [&](int idx)
		{
			ll &f = freq[vc[idx]];
			f++;
			if (f == vc[idx])
				currAns++;
			if (f == vc[idx] + 1)
				currAns--;
		};

		auto remove = [&](int idx)
		{
			ll &f = freq[vc[idx]];
			if (f == vc[idx])
				currAns--;
			if (f == vc[idx] + 1)
				currAns++;
			f--;
		};

		auto getAnswer = [&]()
		{ return currAns; };

		vector<ll> ans = runMo(queries, add, remove, getAnswer);
		for (const auto &x : ans)
			cout << x << endl;
	}
	return 0;
}
