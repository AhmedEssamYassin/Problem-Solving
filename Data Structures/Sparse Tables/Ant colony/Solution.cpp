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
	ll N, L, R;
	cin >> N;
	vector<ll> S(N);
	map<ll, vector<int>> idx;
	for (int i{}; i < N; i++)
		cin >> S[i], idx[S[i]].push_back(i + 1); // Map elements to their indices (1-based)

	SparseTable ST(S, [](ll x, ll y)
	               { return gcd(x, y); }, 0LL);
	cin >> t;
	while (t--)
	{
		cin >> L >> R;
		ll currGCD = ST.query(L - 1, R - 1); // Because Sparse Table is 0-based
		auto &vec = idx[currGCD];
		ll endPos = upper_bound(vec.begin(), vec.end(), R) - vec.begin();
		ll startPos = lower_bound(vec.begin(), vec.end(), L) - vec.begin() + 1;
		ll cnt = endPos - startPos + 1;
		cout << (R - L + 1) - cnt << endl;
	}
	return 0;
}