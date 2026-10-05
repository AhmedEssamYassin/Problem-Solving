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

bool isOk(const vector<ll> &vc, auto &ST, ll K)
{
	bool isSame = true;
	ll OR = ST.query(0, K - 1);
	ll cur{};
	for (int i = 1; i + K - 1 < vc.size(); i++)
	{
		cur = ST.query(i, i + K - 1);
		isSame &= (cur == OR);
	}
	return isSame;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif
	/*
	If we have a valid K for which, each sub-array of length K has the same OR
	then we always can have also that condition held at (K + 1)
	That means this function is monotonic, and we can binary search on K
	But for every candidate K, to check the OR value of each consecutive K elements, we need to do it fast
	We can use a sparse table to do that in O(1)
	*/
	int t = 1;
	ll N;
	cin >> t;
	while (t--)
	{
		cin >> N;
		vector<ll> vc(N);
		for (int i{}; i < N; i++)
			cin >> vc[i];
		SparseTable ST(vc, [](ll a, ll b)
		               { return (a | b); }, 0LL);
		ll L{1}, R = N, ans = N;
		while (L <= R)
		{
			ll mid = ((L + R) >> 1);
			if (isOk(vc, ST, mid))
			{
				ans = mid;
				R = mid - 1;
			}
			else
				L = mid + 1;
		}
		cout << ans << endl;
	}
	return 0;
}