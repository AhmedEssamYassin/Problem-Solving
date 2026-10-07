#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct SplayTree
{
	int size = 1;
	ll value;
	bool reverseProp = false;
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
	void applyReverse() { reverseProp ^= 1, swap(left, right); }
	void push()
	{
		if (reverseProp)
		{
			for (SplayTree *t : {left, right})
				if (t)
					t->applyReverse();
		}
		reverseProp = false;
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
	// cin >> t;
	while (t--)
	{
		ll N, Q, L, R;
		cin >> N >> L >> R;
		vector<ll> arr(N);
		for (int i = 0; i < N; i++)
		{
			char x;
			cin >> x;
			arr[i] = x;
		}
		Tree tree(arr);
		cin >> Q;
		string ans("");
		while (Q--)
		{
			char query;
			char x, y;
			cin >> query;
			if (query == 'S')
			{
				cin >> x >> y;
				if (x == 'L')
					L += 2 * (y == 'R') - 1;
				else if (x == 'R')
					R += 2 * (y == 'R') - 1;
			}
			else if (query == 'R')
				tree.apply(L, R, [](SplayTree *s)
				           { s->applyReverse(); });
			else if (query == 'Q')
			{
				cin >> x;
				if (x == 'L')
					ans += char(tree.apply(L, L, [](SplayTree *s)
					                       { return s->value; }));
				else
					ans += char(tree.apply(R, R, [](SplayTree *s)
					                       { return s->value; }));
			}
		}
		cout << ans;
	}
	return 0;
}