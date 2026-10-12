#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

// Disjoint Sparse Table for any associative operation f (e.g. range product, sum, min, max, etc.).
// Build O(n log n), query O(1). (useful for range product queries when modulo is not prime)
template <typename T, typename F>
struct DisjointSparseTable
{
	vector<vector<T>> t; // t[h][i] = f from i to the midpoint of its 2^(h + 1) block
	F f;
	T id;
	DisjointSparseTable() = default;
	DisjointSparseTable(const vector<T> &a, F f, T id = T()) : f(f), id(id)
	{
		int n = a.size(), LOG = n > 1 ? __lg(n - 1) + 1 : 1;
		t.assign(LOG, a);
		for (int h = 0; h < LOG; h++)
		{
			for (int s = 0, m; s < n; s += 2 << h)
			{
				m = min(n, s + (1 << h));
				for (int i = m - 2; i >= s; i--)
					t[h][i] = f(a[i], t[h][i + 1]);
				for (int i = m + 1; i < min(n, s + (2 << h)); i++)
					t[h][i] = f(t[h][i - 1], a[i]);
			}
		}
	}
	T query(int L, int R) const
	{
		if (L > R)
			return id;
		return L == R ? t[0][L] : f(t[__lg(L ^ R)][L], t[__lg(L ^ R)][R]);
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
	cin >> t;
	while (t--)
	{
		ll n, p, q;
		cin >> n >> p >> q;
		vector<ll> a(n), b(q / 64 + 2);
		for (int i{}; i < n; i++)
			cin >> a[i], a[i] %= p;
		for (int i{}; i < q / 64 + 2; i++)
			cin >> b[i];

		DisjointSparseTable st(a, [&](ll x, ll y)
		                       { return (ll)((__int128)x * y % p); }, 1 % p);

		ll x = 0, L = 0, R = 0;
		for (ll i = 0; i < q; i++)
		{
			if (i % 64 == 0)
				L = (b[i / 64] + x) % n, R = (b[i / 64 + 1] + x) % n;
			else
				L = (L + x) % n, R = (R + x) % n;
			if (L > R)
				swap(L, R);
			x = (st.query(L, R) + 1) % p;
		}
		cout << x << endl;
	}
	return 0;
}
