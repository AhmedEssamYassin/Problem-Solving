#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct SplayTree
{
	int size = 1;
	ll value;
	ll lazyAddValue = 0, subtreeMin;
	bool reverseProp = false;
	SplayTree *left = nullptr, *right = nullptr, *parent = nullptr;

	SplayTree(ll v = 0) : value(v) { update(); }
	static int sz(const SplayTree *t) { return t ? t->size : 0; }
	void update() // inorder: left -> node -> right
	{
		size = 1, subtreeMin = value;
		for (SplayTree *t : {left, right})
			if (t)
				t->parent = this, size += t->size, subtreeMin = min(subtreeMin, t->subtreeMin);
	}
	void applyReverse() { reverseProp ^= 1, swap(left, right); }
	void applyUpdate(ll x) { value += x, lazyAddValue += x, subtreeMin += x; }
	void push()
	{
		for (SplayTree *t : {left, right})
		{
			if (t)
			{
				if (reverseProp)
					t->applyReverse();
				if (lazyAddValue)
					t->applyUpdate(lazyAddValue);
			}
		}
		reverseProp = false, lazyAddValue = 0;
	}
	void rotate() // rotates this node above its parent
	{
		SplayTree *p = parent, *g = p->parent;
		if (g)
			(g->right == p ? g->right : g->left) = this;
		p->push(), push();
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
		return push(), this;
	}
	static SplayTree *merge(SplayTree *L, SplayTree *R)
	{
		if (!L || !R)
			return L ? L : R;
		while (L->push(), L->right)
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
			t->push();
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
	// inserts v before position pos
	static SplayTree *insert(SplayTree *t, int pos, const vector<ll> &v)
	{
		auto [l, r] = split(t, pos - 1);
		return merge(merge(l, build(v, 0, (int)v.size() - 1)), r);
	}
	// right-rotates the sequence by T: swaps [1, N - T] and [N - T + 1, N]
	static void shift(SplayTree *&x, ll T)
	{
		if (!x)
			return;
		ll n = x->size;
		auto [a, b] = split(x, n - (T % n + n) % n);
		x = merge(b, a);
	}
	template <class F>
	static void inorder(SplayTree *t, F f) // visits values left -> node -> right
	{
		vector<SplayTree *> st;
		for (; t || !st.empty(); t = t->right)
		{
			for (; t; t = t->left)
				t->push(), st.push_back(t);
			t = st.back(), st.pop_back(), f(t->value);
		}
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
	void swapRange(int i, int j, int k, int l) { root = SplayTree::swapRange(root, i, j, k, l); }
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
		ll N, M, x, y, v;
		string q;
		cin >> N;
		vector<ll> arr(N);
		for (int i{}; i < N; i++)
			cin >> arr[i];
		Tree tree(arr);
		cin >> M;
		while (M--)
		{
			cin >> q;
			if (q == "ADD")
				cin >> x >> y >> v, tree.apply(x, y, [&](SplayTree *s)
				                               { s->applyUpdate(v); });
			else if (q == "REVERSE")
				cin >> x >> y, tree.apply(x, y, [](SplayTree *s)
				                          { s->applyReverse(); });
			else if (q == "REVOLVE")
				cin >> x >> y >> v, tree.apply(x, y, [&](SplayTree *&s)
				                               { SplayTree::shift(s, v); });
			else if (q == "INSERT")
				cin >> x >> v, tree.insert(x + 1, {v});
			else if (q == "DELETE")
				cin >> x, tree.erase(x, x);
			else if (q == "MIN")
				cin >> x >> y, cout << tree.apply(x, y, [](SplayTree *s)
				                                  { return s->subtreeMin; }, (ll)1e18)
				                    << endl;
		}
	}
	return 0;
}