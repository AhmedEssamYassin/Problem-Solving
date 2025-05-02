#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

const ll mod = 1e9 + 7;

const int BITS = 1000;
struct XORBasis
{
	int sz = 0;
	array<bitset<BITS>, BITS> basis{}; // Better to avoid SHALLOW COPYING
	XORBasis() {}
	XORBasis(const ll &x)
	{
		insertVector(x);
	}
	void insertVector(bitset<BITS> x)
	{
		if (sz == BITS)
			return;
		for (int i = BITS - 1; i >= 0; i--)
		{
			if (x[i])
			{
				if (basis[i].count() == 0)
					return sz++, void(basis[i] = x);
				x ^= basis[i];
			}
		}
	}

	bool canRepresent(bitset<BITS> x)
	{
		if (sz == BITS)
			return true;
		for (int i = BITS - 1; i >= 0; i--)
		{
			if (x[i])
			{
				if (basis[i].count() != 0)
					x ^= basis[i];
				else
					return false;
			}
		}
		return (x.count() == 0);
	}

	ll getMaxXor()
	{
		ll maxXor = 0;
		for (int i = BITS - 1; i >= 0; i--)
		{
			ll b = basis[i].to_ullong(); // Convert bitset to unsigned long long
			if ((maxXor ^ b) > maxXor)
				maxXor ^= b;
		}
		return maxXor;
	}

	friend XORBasis operator+(const XORBasis &LHS, const XORBasis &RHS)
	{
		XORBasis res;
		if (LHS.sz == BITS)
			return LHS;

		if (RHS.sz == BITS)
			return (RHS);
		res += LHS;
		for (int i = BITS - 1; i >= 0; i--)
		{
			if (RHS.basis[i].count())
				res.insertVector(RHS.basis[i]);
		}
		return res;
	}

	XORBasis &operator+=(const XORBasis &other)
	{
		if (sz == BITS)
			return *this;

		if (other.sz == BITS)
			return (*this = other);

		for (int i = BITS - 1; i >= 0; i--)
		{
			if (other.basis[i].count())
				insertVector(other.basis[i]);
		}
		return *this;
	}

	void clear()
	{
		if (!sz)
			return;
		basis.fill(bitset<BITS>());
		sz = 0;
	}
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
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
	int t = 1;
	ll N;
	// cin >> t;
	while (t--)
	{
		// There are at most 168 distinct primes in all numbers until 1000 (Brute forced)
		cin >> N;
		XORBasis xb;
		ll cntDependent{};
		for (int i{}; i < N; i++)
		{
			ll cur;
			cin >> cur;
			bitset<BITS> primeSet;
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