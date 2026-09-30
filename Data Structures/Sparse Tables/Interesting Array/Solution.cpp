#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

template <typename T, typename F>
struct SparseTable
{
	vector<vector<T>> t; // t[k][i] = f over a[i..i + 2^k - 1]
	F f;
	T id; // Returned for an empty range: min -> INF, max -> -INF, gcd / OR -> 0, & -> -1LL, lcm -> 1
	SparseTable() = default;
	SparseTable(const vector<T> &a, F f, T id = T()) : t{a}, f(f), id(id)
	{
		for (int k = 1; (1 << k) <= (int)a.size(); k++)
		{
			t.emplace_back(a.size() - (1 << k) + 1);
			for (int i = 0; i < (int)t[k].size(); i++)
				t[k][i] = f(t[k - 1][i], t[k - 1][i + (1 << (k - 1))]);
		}
	}

	T query(int L, int R) const // f over a[L..R], 0 <= L <= R < n
	{
		if (L > R)
			return id;
		assert(0 <= L && R < (int)t[0].size());
		int k = __lg(R - L + 1);
		return f(t[k][L], t[k][R - (1 << k) + 1]);
	}
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
	ll N, M;
	// cin >> t;
	while (t--)
	{
		cin >> N >> M;
		vector<ll> arr(N, 0);
		vector<tuple<int, int, int>> queries(M);
		for (auto &[l, r, q] : queries)
		{
			cin >> l >> r >> q;
			l--, r--;
		}

		for (int k = 0; k < 31; k++)
		{
			vector<ll> range(N + 1, 0);
			for (const auto &[l, r, q] : queries)
			{
				if (q & (1LL << k))
					range[l]++, range[r + 1]--;
			}
			for (int i{}; i < N; i++)
			{
				if (i)
					range[i] += range[i - 1];
				if (range[i] > 0)
					arr[i] |= (1LL << k);
			}
		}
		SparseTable ST(arr, [](ll x, ll y)
					   { return (x & y); }, -1LL);
		for (auto &[l, r, q] : queries)
			if (ST.query(l, r) != q)
				return cout << "NO", 0;
		cout << "YES\n";
		for (ll &x : arr)
			cout << x << " ";
	}
	return 0;
}