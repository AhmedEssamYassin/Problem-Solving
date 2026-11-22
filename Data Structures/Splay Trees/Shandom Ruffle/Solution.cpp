#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct SplayTree
{
	int size = 1;
	ll value;
	SplayTree *left = nullptr, *right = nullptr, *parent = nullptr;

	SplayTree(ll v = 0) : value(v) { update(); }
	static int sz(const SplayTree *t) { return t ? t->size : 0; }
	void update() // inorder: left -> node -> right
	{
		size = 1;
		for (SplayTree *t : {left, right})
			if (t)
				t->parent = this, size += t->size;
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
	// swaps [i, j] and [k, l] (i <= j < k <= l)
	static SplayTree *swapRange(SplayTree *t, int i, int j, int k, int l)
	{
		auto [a, f, r] = parts(t, i, j);
		auto [m, s, e] = parts(r, k - j, l - j);
		return merge(a, merge(s, merge(m, merge(f, e)))); // [1, i - 1] [k, l] [j + 1, k - 1] [i, j] rest
	}
	template <class F>
	static void inorder(SplayTree *t, F f) // visits values left -> node -> right
	{
		vector<SplayTree *> st;
		for (; t || !st.empty(); t = t->right)
		{
			for (; t; t = t->left)
				st.push_back(t);
			t = st.back(), st.pop_back(), f(t->value);
		}
	}
};

struct Tree
{
	SplayTree *root;

	explicit Tree(SplayTree *r = nullptr) : root(r) {}
	Tree(const vector<ll> &v) : root(SplayTree::build(v, 0, (int)v.size() - 1)) {}
	void swapRange(int i, int j, int k, int l) { root = SplayTree::swapRange(root, i, j, k, l); }
	template <class F>
	void inorder(F f) { SplayTree::inorder(root, f); }
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
		vector<ll> arr(N);
		iota(arr.begin(), arr.end(), 1);
		Tree tree(arr);

		for (int i{}; i < N; i++)
		{
			ll a, b;
			cin >> a >> b;
			if (b <= a)
				continue;

			int len = min(N - b + 1, b - a);

			// swap [a, a + len) and [b, min(b + len, N))
			tree.swapRange(a, a + len - 1, b, b + len - 1);
		}
		tree.inorder([](ll v)
		             { cout << v << " "; });
	}
	return 0;
}