#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

constexpr int BITS = 61, MAX_N = 500;
struct XORBasis
{
	using word = conditional_t<(BITS > 32), uint64_t, uint32_t>;
	using Mask = bitset<MAX_N>;
	array<word, BITS> basis{}; // basis[i] is 0 or has its highest bit at i
	array<Mask, BITS> masks{}; // masks[i] = indices whose XOR equals basis[i]
	int sz = 0;

	bool insertVector(word x, int idx) // true if x was independent of the basis
	{
		Mask m;
		m[idx] = 1;
		for (int i = 0; x && sz < BITS; x ^= basis[i], m ^= masks[i])
			if (!basis[i = __lg(x)])
				return basis[i] = x, masks[i] = m, sz++, true;
		return false;
	}

	bool canRepresent(word x) const
	{
		for (int i = 0; x; x ^= basis[i])
			if (!basis[i = __lg(x)])
				return 0;
		return 1;
	}

	optional<Mask> getIndices(word x) const // a subset of indices XORing to x, nullopt if x is not in the span
	{
		Mask r;
		for (int i = 0; x; x ^= basis[i])
		{
			if (!basis[i = __lg(x)])
				return nullopt;
			else
				r ^= masks[i];
		}
		return r;
	}

	word getMaxXor() const
	{
		word r = 0;
		for (int i = BITS; i--;)
			r = max(r, r ^ basis[i]);
		return r;
	}

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
		ll N, Q;
		cin >> N;
		XORBasis xb;
		for (int i{}; i < N; i++)
		{
			ll x;
			cin >> x;
			xb.insertVector(x, i);
		}
		cin >> Q;
		while (Q--)
		{
			ll x;
			cin >> x;
			if (!xb.canRepresent(x))
				cout << string(N, '0') << endl;
			else
			{
				auto used = xb.getIndices(x);
				for (int i = 0; i < N; i++)
					cout << ((*used)[i]);
				cout << endl;
			}
		}
	}
	return 0;
}