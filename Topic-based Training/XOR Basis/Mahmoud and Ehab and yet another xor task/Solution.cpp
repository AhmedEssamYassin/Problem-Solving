#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

constexpr int BITS = 20;
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

const int mod = 1e9 + 7;
ll modPow(ll N, ll power)
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
	ll N, Q;
	// cin >> t;
	while (t--)
	{
		cin >> N >> Q;
		vector<ll> vc(N);
		for (int i{}; i < N; i++)
			cin >> vc[i];
		vector<XORBasis> acc(N);
		acc[0] = vc[0];
		for (int i = 1; i < N; i++)
			acc[i] = acc[i - 1] + vc[i];

		ll ans{}, l, x;
		while (Q--)
		{
			ans = 0;
			cin >> l >> x;
			if (acc[l - 1].canRepresent(x))
				ans = (modPow(2, l - acc[l - 1].sz));
			cout << ans << endl;
		}
	}
	return 0;
}