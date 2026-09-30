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
	freopen("mex.in", "r", stdin);
	int t = 1;
	ll N, Q;
	cin >> t;
	while (t--)
	{
		/*
		Since the given array is guaranteed to be a permutation, i.e. it includes all numbers [1, N]
		The MEX in any range is obviously the minimum of the remaining of the array
		Example:
		2 6 1 5 3 4
		The MEX in range L = 2, R = 4 --> 2
		The MEX in range L = 4, R = 6 --> 1
		The MEX in range L = 1, R = 6 --> 7 (Special Case)
		*/
		cin >> N;
		cin >> Q;
		vector<ll> a(N);
		for (int i{}; i < N; i++)
			cin >> a[i];

		SparseTable ST(a, [](ll x, ll y)
					   { return min(x, y); }, LLONG_MAX);
		while (Q--)
		{
			ll L, R;
			cin >> L >> R;
			if (R - L + 1 == N)
			{
				cout << N + 1 << endl;
				continue;
			}
			L--, R--;
			int leftQ = (L > 0 ? ST.query(0, L - 1) : INT_MAX);
			int rightQ = (R + 1 < N ? ST.query(R + 1, N - 1) : INT_MAX);
			int val = min(leftQ, rightQ);
			cout << (val ? val : 1) << endl;
		}
	}
	return 0;
}
