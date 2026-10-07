#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

using u64 = uint64_t;
u64 S = chrono::steady_clock::now().time_since_epoch().count() ^ (u64) new char;
u64 rnd()
{
	S += 0xa0761d6478bd642f;
	__uint128_t t = (__uint128_t)S * (S ^ 0xe7037ed1a0b428db);
	return t >> 64 ^ t;
}
#define rng(l, r) ((l) + (int64_t)((__uint128_t)rnd() * ((r) - (l) + 1) >> 64))

const u64 mod = (1LL << 61) - 1;
inline u64 add64(u64 a, u64 b)
{
	a += b;
	return a >= mod ? a - mod : a;
}
inline u64 sub64(u64 a, u64 b) { return a >= b ? a - b : a + mod - b; }
inline u64 mult64(u64 a, u64 b)
{
	__uint128_t t = (__uint128_t)a * b;
	u64 r = u64(t & mod) + u64(t >> 61);
	return r >= mod ? r - mod : r;
}
u64 modPow(u64 n, u64 e)
{
	u64 r = 1;
	for (; e; e >>= 1, n = mult64(n, n))
		if (e & 1)
			r = mult64(r, n);
	return r;
}

// Collision prob ~ n^2 / 2^122 per comparison.
struct H2
{
	u64 a, b;
	H2 operator+(H2 o) const { return {add64(a, o.a), add64(b, o.b)}; }
	H2 operator-(H2 o) const { return {sub64(a, o.a), sub64(b, o.b)}; }
	H2 operator*(H2 o) const { return {mult64(a, o.a), mult64(b, o.b)}; }
	H2 operator+(u64 x) const { return {add64(a, x), add64(b, x)}; }
	H2 operator-(u64 x) const { return {sub64(a, x), sub64(b, x)}; }
	H2 operator*(u64 x) const { return {mult64(a, x), mult64(b, x)}; }
	auto operator<=>(const H2 &) const = default;
};
const H2 B = {(u64)rng(1 << 20, mod - 2), (u64)rng(1 << 20, mod - 2)};
const H2 BI = {modPow(B.a, mod - 2), modPow(B.b, mod - 2)};
auto [Pb, sumB] = [](int mx)
{
	vector<H2> p(mx + 1, {1, 1}), s(mx + 1, {1, 1});
	for (int i = 1; i <= mx; i++)
	{
		p[i] = p[i - 1] * B;
		s[i] = s[i - 1] + p[i];
	}
	return pair{move(p), move(s)};
}(7e5);

struct Hash
{
	H2 code = {0, 0};
	int size = 0;
	Hash() = default;
	Hash(H2 c, int s) : code(c), size(s) {}
	Hash(ll x) : size(1)
	{
		u64 xm = ((x % (ll)mod) + mod) % mod;
		code = {xm, xm};
	}
	Hash(string_view s) : size(s.size())
	{
		for (uint8_t c : s)
			code = code * B + c;
	}
	void clear() { code = {0, 0}, size = 0; }

	Hash operator+(const Hash &o) const { return {code * Pb[o.size] + o.code, size + o.size}; }
	auto operator<=>(const Hash &) const = default;
};
// Rabin-Karp
struct HashRange
{
	vector<H2> p, s;
	HashRange() = default;
	HashRange(string_view t) : p(t.size() + 1, {0, 0}), s(t.size() + 1, {0, 0})
	{
		int n = t.size();
		for (int i = 0; i < n; i++)
		{
			p[i + 1] = p[i] * B + (uint8_t)t[i];
			s[n - 1 - i] = s[n - i] * B + (uint8_t)t[n - 1 - i];
		}
	}

	Hash get(int l, int r) const { return l <= r ? Hash{p[r + 1] - p[l] * Pb[r - l + 1], r - l + 1} : Hash{}; }
	Hash inv(int l, int r) const { return l <= r ? Hash{s[l] - s[r + 1] * Pb[r - l + 1], r - l + 1} : Hash{}; }
};

struct SplayTree
{
	int size = 1;
	ll value;
	Hash hash;
	SplayTree *left = nullptr, *right = nullptr, *parent = nullptr;

	SplayTree(ll v = 0) : value(v) { update(); }
	static int sz(const SplayTree *t) { return t ? t->size : 0; }
	void update() // inorder: left -> node -> right
	{
		size = 1, hash = value;
		if (left)
			left->parent = this, size += left->size, hash = left->hash + hash;
		if (right)
			right->parent = this, size += right->size, hash = hash + right->hash;
	}
	void rotate() // rotates this node above its parent
	{
		SplayTree *p = parent, *g = p->parent;
		if (g)
			(g->right == p ? g->right : g->left) = this;
		if (p->left == this)
			p->left = right, right = p;
		else
			p->right = left, left = p;
		parent = g, p->update(), update();
	}
	SplayTree *splay()
	{
		while (parent)
		{
			SplayTree *p = parent, *g = p->parent;
			if (g)
				((g->right == p) == (p->right == this) ? p : this)->rotate();
			rotate();
		}
		return this;
	}
	static SplayTree *merge(SplayTree *L, SplayTree *R)
	{
		if (!L || !R)
			return L ? L : R;
		while (L->right)
			L = L->right;
		L->splay()->right = R;
		return L->update(), L;
	}
	// kth element (1-based), splayed to root; nullptr if k invalid
	static SplayTree *findKth(SplayTree *t, int k)
	{
		if (k < 1 || k > sz(t))
			return nullptr;
		for (;;)
		{
			int ls = sz(t->left);
			if (k == ls + 1)
				return t->splay();
			if (k <= ls)
				t = t->left;
			else
				k -= ls + 1, t = t->right;
		}
	}
	// splits into [1, k] and [k + 1, size]
	static pair<SplayTree *, SplayTree *> split(SplayTree *t, int k)
	{
		if (k <= 0 || !t)
			return {nullptr, t};
		if (k >= t->size)
			return {t, nullptr};
		t = findKth(t, k);
		SplayTree *R = t->right;
		R->parent = t->right = nullptr;
		return t->update(), pair{t, R};
	}
	static SplayTree *build(const vector<ll> &v, int l, int r)
	{
		if (l > r)
			return nullptr;
		int m = (l + r) / 2;
		SplayTree *t = new SplayTree(v[m]);
		t->left = build(v, l, m - 1), t->right = build(v, m + 1, r);
		return t->update(), t;
	}
	// [1, L - 1], [L, R], [R + 1, size]
	static array<SplayTree *, 3> parts(SplayTree *t, int L, int R)
	{
		auto [l, m] = split(t, L - 1);
		auto [s, r] = split(m, R - L + 1);
		return {l, s, r};
	}
	// inserts v before position pos
	static SplayTree *insert(SplayTree *t, int pos, const vector<ll> &v)
	{
		auto [l, r] = split(t, pos - 1);
		return merge(merge(l, build(v, 0, (int)v.size() - 1)), r);
	}
	static void freeNodes(SplayTree *t)
	{
		vector<SplayTree *> st{t};
		while (!st.empty())
		{
			SplayTree *x = st.back();
			st.pop_back();
			if (x)
				st.push_back(x->left), st.push_back(x->right), delete x;
		}
	}
};

struct Tree
{
	SplayTree *root;

	explicit Tree(SplayTree *r = nullptr) : root(r) {}
	Tree(const vector<ll> &v) : root(SplayTree::build(v, 0, (int)v.size() - 1)) {}
	void insert(int pos, const vector<ll> &v) { root = SplayTree::insert(root, pos, v); }
	void erase(int L, int R)
	{
		auto [l, s, r] = SplayTree::parts(root, L, R);
		SplayTree::freeNodes(s), root = SplayTree::merge(l, r);
	}
	template <class R>
	using IdT = conditional_t<is_void_v<R>, nullptr_t, R>; // f's result type; dummy if void
	// Runs f(seg) on [L, R] and re-attaches it; returns f's result (or id when empty) or void.
	template <class F>
	auto apply(int L, int R, F f, IdT<invoke_result_t<F, SplayTree *&>> id = {})
	{
		using Res = invoke_result_t<F, SplayTree *&>;
		auto [l, s, r] = SplayTree::parts(root, L, R);
		if constexpr (is_void_v<Res>)
		{
			if (s)
				f(s);
			root = SplayTree::merge(l, SplayTree::merge(s, r));
		}
		else
		{
			Res res = s ? f(s) : id;
			return root = SplayTree::merge(l, SplayTree::merge(s, r)), res;
		}
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
		ll N, Q;
		cin >> N >> Q;
		string str;
		cin >> str;
		vector<ll> forArr(N), backArr(N);
		for (int i = 0; i < N; i++)
		{
			forArr[i] = str[i] - 'a';
			backArr[i] = str[N - 1 - i] - 'a';
		}
		Tree forTree(forArr), backTree(backArr);

		for (int i{}; i < Q; i++)
		{
			ll query, L, R, p;
			char c;
			cin >> query;
			if (query == 1)
			{
				cin >> L >> R;
				forTree.erase(L, R);
				backTree.erase(N - R + 1, N - L + 1);
				N -= (R - L + 1);
			}
			else if (query == 2)
			{
				cin >> c >> p;
				forTree.insert(p, {c - 'a'});
				backTree.insert(N - p + 2, {c - 'a'});
				N++;
			}
			else if (query == 3)
			{
				cin >> L >> R;
				Hash fHash, bHash;
				forTree.apply(L, R, [&](SplayTree *s)
				              { fHash = s->hash; });
				backTree.apply(N - R + 1, N - L + 1, [&](SplayTree *s)
				               { bHash = s->hash; });
				if (fHash == bHash)
					cout << "yes\n";
				else
					cout << "no\n";
			}
		}
	}
	return 0;
}