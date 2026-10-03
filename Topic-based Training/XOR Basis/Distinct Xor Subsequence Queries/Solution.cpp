#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

constexpr int BITS = 64;
struct XORBasis
{
	using word = conditional_t<(BITS > 32), uint64_t, uint32_t>;
	array<word, BITS> basis{}; // basis[i] is 0 or has its highest bit at i
	int sz = 0;

	XORBasis(word x = 0) { insertVector(x); }

	bool insertVector(word x) // true if x was independent of the basis
	{
		for (int i = 0; x && sz < BITS; x ^= basis[i])
			if (!basis[i = __lg(x)])
				return basis[i] = x, sz++, true;
		return false;
	}

	bool canRepresent(word x) const
	{
		for (int i = 0; x; x ^= basis[i])
			if (!basis[i = __lg(x)])
				return 0;
		return 1;
	}

	void reduce() // RREF in place, span unchanged
	{
		word piv = 0; // bitmask of pivot columns
		for (int i = 0; i < BITS; i++)
			if (basis[i])
				piv |= word(1) << i;
		for (int i = 0; i < BITS; i++)
			if (basis[i])
				for (word m = basis[i] & piv & ((word(1) << i) - 1); m; m &= m - 1)
					basis[i] ^= basis[__builtin_ctzll(m)];
	}
	bool hasKth(word k) const { return sz >= numeric_limits<word>::digits || !(k >> sz); }
	// k-th smallest element of the span, 0-indexed (k = 0 gives 0). Requires k < 2^sz.
	word kthSmallest(word k)
	{
		reduce();
		word r = 0;
		for (word v : basis)
			if (v)
				r ^= v * (k & 1), k >>= 1;
		return r;
	}

	word getMaxXor() const
	{
		word r = 0;
		for (int i = BITS; i--;)
			r = max(r, r ^ basis[i]);
		return r;
	}

	XORBasis &operator+=(const XORBasis &o)
	{
		if (o.sz == BITS) // Full basis spans everything
			return *this = o;
		for (word v : o.basis)
			insertVector(v);
		return *this;
	}
	friend XORBasis operator+(XORBasis a, const XORBasis &b) { return a += b; }

	void clear() { *this = {}; }
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
	// cin >> t;
	while (t--)
	{
		ll N;
		cin >> N;
		XORBasis xb;
		for (int i{}; i < N; i++)
		{
			ll type, x, k;
			cin >> type;
			if (type == 1)
			{
				cin >> x;
				xb.insertVector(x);
			}
			else
			{
				cin >> k;
				--k;
				if (!xb.hasKth(k))
					cout << -1 << endl;
				else
					cout << xb.kthSmallest(k) << endl;
			}
		}
	}
	return 0;
}