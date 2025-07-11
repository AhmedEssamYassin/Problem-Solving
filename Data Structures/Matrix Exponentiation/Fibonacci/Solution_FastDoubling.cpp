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
void fastDoubling(ll N, ll &x, ll &y)
{
	if (N == 0)
	{
		x = 0;
		y = 1;
		return;
	}

	if (N & 1)
	{
		fastDoubling(N - 1, y, x);
		y = add64(y, x);
	}
	else
	{
		ll a, b;
		fastDoubling(N >> 1, a, b);

		ll a_sq = mult64(a, a);
		ll b_sq = mult64(b, b);
		ll ab = mult64(a, b);
		ll b_minus_a = sub64(b, a);

		y = add64(a_sq, b_sq);
		x = add64(ab, mult64(a, b_minus_a));
	}
}

ll F(ll N)
{
	ll x, y;
	fastDoubling(N, x, y); // x = F(N), y = F(N + 1)
	return x;
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
	ll N;
	// cin >> t;
	while (t--)
	{
		cin >> N;
		cout << F(N);
	}
	return 0;
}