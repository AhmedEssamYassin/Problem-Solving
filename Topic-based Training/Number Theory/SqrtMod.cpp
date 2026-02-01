#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

inline ll add64(const ll &a, const ll &b, const ll &mod)
{
	__int128_t res = __int128_t(a) + b;
	if (res >= mod)
		res -= mod;
	return res;
}

inline ll sub64(const ll &a, const ll &b, const ll &mod)
{
	__int128_t res = __int128_t(a) - b;
	if (res < 0)
		res += mod;
	if (res >= mod)
		res -= mod;
	return res;
}

inline ll mult64(const ll &a, const ll &b, const ll &mod)
{
	return __int128_t(a) * b % mod;
}

ll modPow(ll N, ll power, const ll &mod)
{
	if (N % mod == 0 || N == 0)
		return 0;
	if (N == 1 || power == 0)
		return 1;

	ll res{1};
	while (power)
	{
		if (power & 1)
			res = mult64(res, N, mod);
		N = mult64(N, N, mod);
		power >>= 1;
	}
	return res;
}

inline ll normalize(ll n, const ll &mod)
{
	n %= mod;
	(n < 0) && (n += mod);
	return n;
}

ll sqrtMod(ll x, ll p)
{
	x = normalize(x, p);
	if (x == 0)
		return 0;
	if (x == 1)
		return 1;

	// Check if x is a quadratic residue using Euler's criterion
	if (modPow(x, (p - 1) / 2, p) != 1)
		return -1; // No solution exists

	// Special case for p ≡ 3 (mod 4)
	if (p % 4 == 3)
		return modPow(x, (p + 1) / 4, p);

	// Tonelli-Shanks algorithm for general case
	ll Q = p - 1;
	ll S = 0;
	while (Q % 2 == 0)
	{
		Q /= 2;
		S++;
	}

	if (S == 1)
		return modPow(x, (p + 1) / 4, p);

	// Find a quadratic non-residue z
	ll z = 2;
	while (modPow(z, (p - 1) / 2, p) != p - 1)
		z++;

	ll M = S;
	ll c = modPow(z, Q, p);
	ll t = modPow(x, Q, p);
	ll R = modPow(x, (Q + 1) / 2, p);

	while (t != 1)
	{
		ll i = 1;
		ll temp = mult64(t, t, p);
		while (temp != 1 && i < M)
		{
			temp = mult64(temp, temp, p);
			i++;
		}

		ll b = modPow(c, 1LL << (M - i - 1), p);
		M = i;
		c = mult64(b, b, p);
		t = mult64(t, c, p);
		R = mult64(R, b, p);
	}

	return R;
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
	ll Y, P;
	cin >> t;
	while (t--)
	{
		cin >> Y >> P;
		cout << sqrtMod(Y, P) << endl;
	}
	return 0;
}