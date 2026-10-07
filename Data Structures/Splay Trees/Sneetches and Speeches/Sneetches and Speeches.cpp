#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct S // count / longest prefix / suffix / run of each bit over a sequence
{
	int size = 0, c[2]{}, pre[2]{}, suf[2]{}, ans[2]{};
	friend S operator+(const S &a, const S &b)
	{
		S r;
		r.size = a.size + b.size;
		for (int i : {0, 1})
		{
			r.c[i] = a.c[i] + b.c[i];
			r.pre[i] = a.pre[i] == a.size ? a.size + b.pre[i] : a.pre[i];
			r.suf[i] = b.suf[i] == b.size ? b.size + a.suf[i] : b.suf[i];
			r.ans[i] = max({a.ans[i], b.ans[i], a.suf[i] + b.pre[i]});
		}
		return r;
	}
};

struct SplayTree : S
{
	ll value;
	ll assignValue = -1;
	bool flipProp = false, reverseProp = false;
	SplayTree *left = nullptr, *right = nullptr, *parent = nullptr;

	SplayTree(ll v = 0) : value(v) { update(); }
	static int sz(const SplayTree *t) { return t ? t->size : 0; }
	void update() // inorder: left -> node -> right
	{
		S l, m, r;
		m.size = 1, m.c[value] = m.pre[value] = m.suf[value] = m.ans[value] = 1;
		if (left)
			left->parent = this, l = *left;
		if (right)
			right->parent = this, r = *right;
		S::operator=(l + m + r);
	}
	void applyReverse() { reverseProp ^= 1, swap(left, right), swap_ranges(pre, pre + 2, suf); }
	void applyFlip()
	{
		value ^= 1, flipProp ^= 1;
		for (auto *a : {c, pre, suf, ans})
			swap(a[0], a[1]);
	}
	void applyAssign(int x) // sets the whole subtree to x (cancels pending flip / reverse)
	{
		value = x, assignValue = x, flipProp = reverseProp = false;
		for (int b : {0, 1})
			c[b] = pre[b] = suf[b] = ans[b] = b == x ? size : 0;
	}
	void push()
	{
		for (SplayTree *t : {left, right})
		{
			if (t)
			{
				if (assignValue >= 0)
					t->applyAssign(assignValue);
				if (flipProp)
					t->applyFlip();
				if (reverseProp)
					t->applyReverse();
			}
		}
		assignValue = -1, flipProp = reverseProp = false;
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
		ll N, Q;
		cin >> N >> Q;
		vector<ll> arr(N);
		for (int i = 0; i < N; i++)
		{
			char c;
			cin >> c;
			arr[i] = c - '0';
		}
		Tree tree(arr);
		for (int i{}; i < Q; i++)
		{
			ll type, L, R;
			cin >> type >> L >> R;
			if (type == 1)
				tree.apply(L, R, [](SplayTree *s)
				           { s->applyFlip(); });
			else if (type == 2)
				tree.apply(L, R, [](SplayTree *s)
				           { s->applyReverse(); });
			else
				tree.apply(L, R, [](SplayTree *&seg)
				           {
							   auto [zeros, ones] = SplayTree::split(seg, seg->c[0]);
							   if (zeros)
								   zeros->applyAssign(0);
							   if (ones)
								   ones->applyAssign(1);
							   seg = SplayTree::merge(zeros, ones); });
			cout << max(tree.root->ans[0], tree.root->ans[1]) << endl;
		}
	}
	return 0;
}