#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

// Basis of a[1..r] that keeps, for each pivot, the latest possible position.
// The rows with pos >= l span exactly a[l..r]. Positions are 1-indexed (pos = 0 marks an empty row),
// must be inserted in increasing order, and queries refer to the current r (l >= 1).
constexpr int BITS = 64;
struct PrefixBasis
{
	using word = conditional_t<(BITS > 32), uint64_t, uint32_t>;
	array<word, BITS> b{};
	array<int, BITS> pos{};

	void insert(word x, int p)
	{
		for (int i; x; x ^= b[i])
		{
			if (!b[i = __lg(x)])
			{
				b[i] = x, pos[i] = p;
				return;
			}
			else if (pos[i] < p)
				swap(b[i], x), swap(pos[i], p); // keep the newer vector as the pivot
		}
	}

	bool canRepresent(word x, int l) const
	{
		for (int i = 0; x; x ^= b[i])
			if (pos[i = __lg(x)] < l)
				return false;
		return true;
	}

	word getMaxXor(int l) const
	{
		word r = 0;
		for (int i = BITS; i--;)
			if (pos[i] >= l)
				r = max(r, r ^ b[i]);
		return r;
	}

	int rank(int l) const // dimension of the span of a[l..r]
	{
		int c = 0;
		for (const int &p : pos)
			c += (p >= l);
		return c;
	}
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
		int n, q;
		cin >> n;
		vector<PrefixBasis> pref(n + 1);
		for (int r = 1; r <= n; r++)
		{
			ll x;
			cin >> x;
			pref[r] = pref[r - 1];
			pref[r].insert(x, r);
		}
		cin >> q;
		while (q--)
		{
			int l, r;
			ll x;
			cin >> l >> r >> x;
			cout << (pref[r].canRepresent(x, l) ? "Yes" : "No") << endl;
		}
	}
	return 0;
}