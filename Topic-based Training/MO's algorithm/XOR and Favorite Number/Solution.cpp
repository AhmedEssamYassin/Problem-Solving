#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

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
		int n, m, k;
		cin >> n >> m >> k;
		vector<ll> vc(n);
		for (int i{}; i < n; i++)
			cin >> vc[i];
		vector<ll> prefXOR(n + 1);
		prefXOR[0] = 0;
		for (int i = 0; i < n; i++)
			prefXOR[i + 1] = prefXOR[i] ^ vc[i];

		vector<Query> queries;
		queries.reserve(m);
		for (int i = 0; i < m; i++)
		{
			int l, r;
			cin >> l >> r;
			--l; // Because prefix xor of range [l, r] is (prefix[r] ^ prefix[l - 1])

			queries.emplace_back(l, r, i); // hilbert computed once
		}

		ll currAns{};
		vector<ll> freq(2e6 + 1, 0);
		auto add = [&](int idx)
		{
			ll pref = prefXOR[idx];
			ll x = pref ^ k;
			currAns += freq[x];
			freq[pref]++;
		};

		auto remove = [&](int idx)
		{
			ll pref = prefXOR[idx];
			freq[pref]--;
			ll x = pref ^ k;
			currAns -= freq[x];
		};

		auto getAnswer = [&]()
		{ return currAns; };

		vector<ll> ans = runMo(queries, add, remove, getAnswer);
		for (const auto &x : ans)
			cout << x << endl;
	}
	return 0;
}