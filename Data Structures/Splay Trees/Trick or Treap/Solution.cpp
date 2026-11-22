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
		ll Q;
		cin >> Q;
		vector<SplayTree *> spTrees(Q + 1);
		auto root = [&](ll i)
		{ return spTrees[i]->splay(); };
		for (int i{1}; i <= Q; i++)
		{
			ll query, y, z;
			cin >> query;
			if (query == 1)
			{
				cin >> y;
				spTrees[i] = new SplayTree(y);
			}
			else if (query == 2)
			{
				cin >> y >> z;
				auto a = root(y), b = root(z);
				// after splaying z, y is still a root only if it lives in another tree
				if (a != b && !a->parent)
					SplayTree::merge(a, b);
			}
			else if (query == 3)
			{
				cin >> y >> z;
				SplayTree::split(root(y), z);
			}
			else
			{
				cin >> y;
				cout << root(y)->subtreeSum << endl;
			}
		}
	}
	return 0;
}