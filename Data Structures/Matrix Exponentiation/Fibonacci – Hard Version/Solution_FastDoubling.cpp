#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

const ll mod = 1e9 + 7;
#define double_size_t std::conditional_t<(mod > (1LL << 31)), __int128_t, long long>
inline ll add64(const ll &a, const ll &b)
{
	double_size_t res = double_size_t(a) + b;
	if (res >= mod)
		res -= mod;
	return res;
}

inline ll sub64(const ll &a, const ll &b)
{
	double_size_t res = double_size_t(a) - b;
	if (res < 0)
		res += mod;
	if (res >= mod)
		res -= mod;
	return res;
}

inline ll mult64(const ll &a, const ll &b)
{
	return double_size_t(a) * b % mod;
}

// Fast Doubling method
pair<ll, ll> getNthFib(ll N)
{
	ll a = 0, b = 1; // (F(0), F(1))
	if (N == 0)
		return {a, b};

	int msb = __lg(N);
	for (int i = msb; i >= 0; i--)
	{
		ll c = mult64(a, sub64(mult64(b, 2), a)); // F(2k)
		ll d = add64(mult64(a, a), mult64(b, b)); // F(2k + 1)
		if ((N >> i) & 1)
		{
			a = d;
			b = add64(c, d);
		}
		else
		{
			a = c;
			b = d;
		}
	}
	return {a, b};
}

ll F(ll N)
{
	return getNthFib(N).first;
}

ll sumFib(ll N)
{
	return F(N + 2) - 2;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
	int t = 1;
	ll L, R;
	cin >> t;
	while (t--)
	{
		cin >> L >> R;
		cout << sub64(sumFib(R), sumFib(L - 1)) << endl;
	}
	return 0;
}