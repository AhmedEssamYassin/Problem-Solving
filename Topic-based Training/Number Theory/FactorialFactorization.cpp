#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define u64 uint64_t
#define u128 __uint128_t
#define endl "\n"

inline u64 mult64(u64 a, u64 b, u64 mod) { return (u128)a * b % mod; }

template <class F>
void pff(ll n, F &&f)
{
	if (n < 2)
		return;
	{
		ll e = 0;
		for (ll m = n; m;)
			e += (m >>= 1);
		f(2LL, e);
	}

	ll h = (n - 1) / 2;
	vector<bool> comp(h + 1);
	for (ll i = 1; (2 * i + 1) * (2 * i + 1) <= n; i++)
	{
		if (!comp[i])
		{
			for (ll p = 2 * i + 1, j = p * p / 2; j <= h; j += p)
				comp[j] = true;
		}
	}
	for (ll i = 1; i <= h; i++)
	{
		if (!comp[i])
		{
			ll p = 2 * i + 1, e = 0;
			for (ll m = n; m;)
				e += (m /= p);
			f(p, e);
		}
	}
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
	cin >> t;
	while (t--)
	{
		ll n;
		constexpr int mod = 1e9 + 7;
		cin >> n;
		uint32_t cnt = 1;
		pff(n, [&](ll p, ll e)
		    { cnt = mult64(cnt, e + 1, mod); });
		cout << cnt << endl;
	}
	return 0;
}
