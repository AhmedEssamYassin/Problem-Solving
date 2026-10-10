#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

template <typename T>
struct ChthollyTree
{
	struct Node
	{
		ll l;
		mutable ll r;
		mutable T val;
		Node(ll l_, ll r_, T val_) : l(l_), r(r_), val(val_) {}
		bool operator<(const Node &other) const { return l < other.l; }
	};

	set<Node> tree;

	ChthollyTree() = default;
	explicit ChthollyTree(const vector<T> &a) { init(a); }
	ChthollyTree(ll n, T val) { init(n, val); }

	void init(const vector<T> &a)
	{
		tree.clear();
		ll n = a.size();
		for (ll i = 0; i < n; ++i)
			tree.emplace_hint(tree.end(), i, i, a[i]);
		tree.emplace_hint(tree.end(), n, n, T{}); // sentinel
	}

	void init(ll n, T val)
	{
		tree.clear();
		if (n > 0)
			tree.emplace(0, n - 1, val);
		tree.emplace_hint(tree.end(), n, n, T{}); // sentinel
	}

	typename set<Node>::iterator split(ll pos)
	{
		auto it = tree.lower_bound(Node(pos, -1, T{}));
		if (it != tree.end() && it->l == pos)
			return it;
		--it;
		ll r = it->r;
		T val = it->val;
		it->r = pos - 1;
		return tree.emplace_hint(next(it), pos, r, val);
	}

	void rngAssign(ll l, ll r, T val)
	{
		auto itr = split(r + 1);
		auto itl = split(l);
		tree.erase(next(itl), itr);
		itl->r = r;
		itl->val = val;
	}

	void rngAdd(ll l, ll r, T delta)
	{
		auto itr = split(r + 1);
		auto itl = split(l);
		for (auto it = itl; it != itr; ++it)
			it->val += delta;
	}

	template <typename F>
	void rngApply(ll l, ll r, F f)
	{
		auto itr = split(r + 1);
		auto itl = split(l);
		for (auto it = itl; it != itr; ++it)
			f(it->l, it->r, it->val);
	}

	T pointQuery(ll pos) const
	{
		auto it = tree.upper_bound(Node(pos, -1, T{}));
		--it; // last node with l <= pos
		return it->val;
	}

	T kth(ll l, ll r, ll k) // k-th smallest value (1-based) in [l..r]
	{
		auto itr = split(r + 1);
		auto itl = split(l);
		vector<pair<T, ll>> segs;
		segs.reserve(distance(itl, itr));
		for (auto it = itl; it != itr; ++it)
			segs.emplace_back(it->val, it->r - it->l + 1);
		sort(segs.begin(), segs.end());
		for (auto &[val, len] : segs)
		{
			if (k <= len)
				return val;
			k -= len;
		}
		throw out_of_range("ChthollyTree::kth: k exceeds range size");
	}

	ll rngSumPow(ll l, ll r, ll x, ll mod)
	{
		auto itr = split(r + 1);
		auto itl = split(l);
		ll res = 0;
		for (auto it = itl; it != itr; ++it)
		{
			ll len = (it->r - it->l + 1) % mod;
			res = (res + mult64(len, modPow((ll)it->val, x, mod), mod)) % mod;
		}
		return res;
	}

	T rngSum(ll l, ll r)
	{
		auto itr = split(r + 1);
		auto itl = split(l);
		T res = T{};
		for (auto it = itl; it != itr; ++it)
			res = res + it->val * (ll)(it->r - it->l + 1);
		return res;
	}

	size_t intervalCount() const { return tree.size() - 1; }

private:
	static inline T mult64(const T &a, const T &b, const T &mod)
	{
		return __int128_t(a) * b % mod;
	}

	static T modPow(T N, T power, T mod)
	{
		if (N % mod == 0 || N == 0)
			return 0;
		if (N == 1 || power == 0)
			return 1;
		T res{1};
		while (power)
		{
			if (power & 1)
				res = mult64(res, N, mod);
			N = mult64(N, N, mod);
			power >>= 1;
		}
		return res;
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
		ll n, m, seed, vMax;
		cin >> n >> m >> seed >> vMax;
		vector<ll> vc(n);
		auto rnd = [&]()
		{
			ll ret = seed;
			seed = (seed * 7 + 13) % 1000000007;
			return ret;
		};

		for (int i = 0; i < n; i++)
			vc[i] = (rnd() % vMax) + 1;
		ChthollyTree<ll> odt(vc);

		for (int i = 0; i < m; i++)
		{
			int op = (rnd() % 4) + 1;
			int l = (rnd() % n);
			int r = (rnd() % n);
			int x, y;
			if (l > r)
				swap(l, r);

			if (op == 3)
				x = (rnd() % (r - l + 1)) + 1;
			else
				x = (rnd() % vMax) + 1;

			if (op == 4)
				y = (rnd() % vMax) + 1;

			if (op == 1)
				odt.rngAdd(l, r, x);
			else if (op == 2)
				odt.rngAssign(l, r, x);
			else if (op == 3)
				cout << odt.kth(l, r, x) << endl;
			else
				cout << odt.rngSumPow(l, r, x, y) << endl;
		}
	}
	return 0;
}
