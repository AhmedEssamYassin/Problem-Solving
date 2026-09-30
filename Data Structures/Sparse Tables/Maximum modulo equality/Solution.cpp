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
	ll N, Q;
	cin >> t;
	while (t--)
	{
		cin >> N >> Q;
		vector<ll> vc(N), diff;
		for (int i{}; i < N; i++)
			cin >> vc[i];
		for (int i{1}; i < N; i++)
			diff.push_back(abs(vc[i] - vc[i - 1]));
		SparseTable SPT(diff, [](ll x, ll y)
						{ return gcd(x, y); }, 0LL);
		while (Q--)
		{
			int L, R;
			cin >> L >> R;
			--L, --R;
			cout << SPT.query(L, R - 1) << " ";
		}
		cout << endl;
	}
	return 0;
}