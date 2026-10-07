#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct SplayTree
{
	int size = 1;
	ll value;
	ll subtreeSum;
	SplayTree *left = nullptr, *right = nullptr, *parent = nullptr;

	SplayTree(ll v = 0) : value(v) { update(); }
	static int sz(const SplayTree *t) { return t ? t->size : 0; }
	void update() // inorder: left -> node -> right
	{
		size = 1, subtreeSum = value;
		for (SplayTree *t : {left, right})
			if (t)
				t->parent = this, size += t->size, subtreeSum += t->subtreeSum;
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
};

struct Tree
{
	SplayTree *root;

	explicit Tree(SplayTree *r = nullptr) : root(r) {}
	Tree(const vector<ll> &v) : root(SplayTree::build(v, 0, (int)v.size() - 1)) {}
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
	ll N, M, q, i, v, L, R;
	cin >> N >> M;
	vector<ll> arr(N);
	for (int i{}; i < N; i++)
		cin >> arr[i];
	Tree tree(arr);

	while (M--)
	{
		cin >> q;
		if (q == 1)
		{
			cin >> i >> v;
			tree.apply(i + 1, i + 1, [&](SplayTree *s)
			           { s->value = s->subtreeSum = v; });
		}
		else
		{
			cin >> L >> R;
			cout << tree.apply(L + 1, R, [](SplayTree *s)
			                   { return s->subtreeSum; })
			     << endl;
		}
	}

	return 0;
}