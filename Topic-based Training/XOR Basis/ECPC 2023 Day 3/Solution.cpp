#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

const ll mod = 1e9 + 7;

constexpr int BITS = 1000;
struct XORBasis
{
	using Mask = bitset<BITS>;
	array<Mask, BITS> basis{}; // basis[i] is 0 or has its highest bit at i
	int sz = 0;

	XORBasis(const Mask &x = {}) { insertVector(x); }

	bool insertVector(Mask x) // true if x was independent of the basis
	{
		for (int i = BITS; i-- && sz < BITS;)
		{
			if (x[i])
			{
				if (!basis[i][i])
					return basis[i] = x, sz++, true;
				x ^= basis[i];
			}
		}
		return false;
	}

	bool canRepresent(Mask x) const
	{
		for (int i = BITS; i--;)
			if (x[i] && (x ^= basis[i])[i]) // still set means no pivot at i
				return 0;
		return 1;
	}

	Mask getMaxXor() const
	{
		Mask r;
		for (int i = BITS; i--;)
			if (!r[i])
				r ^= basis[i];
		return r;
	}

	XORBasis &operator+=(const XORBasis &o)
	{
		if (o.sz == BITS) // Full basis spans everything
			return *this = o;
		for (int i = 0; i < BITS && sz < BITS; i++)
			if (o.basis[i][i])
				insertVector(o.basis[i]);
		return *this;
	}
	friend XORBasis operator+(XORBasis a, const XORBasis &b) { return a += b; }

	void clear() { *this = {}; }
};

ll modPow(ll N, ll power, ll mod)
{
	ll res{1};
	while (power)
	{
		if (power & 1)
			res = (res % mod * N % mod) % mod;
		N = (N % mod * N % mod) % mod;
		power >>= 1;
	}
	return res;
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
	// cin >> t;
	while (t--)
	{
		ll N;
		// There are at most 168 distinct primes in all numbers until 1000 (Brute forced)
		cin >> N;
		XORBasis xb;
		ll cntDependent{};
		for (int i{}; i < N; i++)
		{
			ll cur;
			cin >> cur;
			bitset<1000> primeSet;
			for (ll p = 2; p * p <= cur && cur > 1; p++)
			{
				while (cur % p == 0)
					cur /= p, primeSet.flip(p);
			}
			if (cur > 1)
				primeSet.flip(cur);
			if (xb.canRepresent(primeSet))
				cntDependent++;
			else
				xb.insertVector(primeSet);
		}
		cout << (modPow(2, cntDependent, mod) - 1 + mod) % mod;
	}
	return 0;
}