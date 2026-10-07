#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct LazySegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		ll value;
		Node() {}
		Node(const ll &N) : value(N) {}
		Node operator+(const Node &RHS)
		{
			value += RHS.value;
			return *this;
		}
	};
	int size;
	vector<Node> seg;
	vector<Node> lazy;
	Node merge(const Node &leftNode, const Node &rightNode)
	{
		Node res = (leftNode.value + rightNode.value);
		return res;
	}
	void build(int left, int right, int node, const vector<ll> &arr)
	{
		// If the segment has only one element, leaf node
		if (left == right)
		{
			if (left < arr.size())
				seg[node] = arr[left];
			return;
		}
		// Recursively build the left child
		build(left, mid, L, arr);
		// Recursively build the right child
		build(mid + 1, right, R, arr);
		// Merge the children values
		seg[node] = merge(seg[L], seg[R]);
	}
	void push(int left, int right, int node)
	{
		// Propagate the value
		if (lazy[node].value == 0)
			return;
		seg[node].value += (right - left + 1) * lazy[node].value;
		// If the node is not a leaf
		if (left != right)
		{
			// Update the lazy values for the left child
			lazy[L].value += lazy[node].value;
			// Update the lazy values for the right child
			lazy[R].value += lazy[node].value;
		}
		// Reset the lazy value
		lazy[node] = 0;
	}
	void update(int left, int right, int node, int leftQuery, int rightQuery, const ll &val)
	{
		push(left, right, node);
		// If the range is invalid, return
		if (left > rightQuery || right < leftQuery)
			return;
		// If the range matches the segment
		if (left >= leftQuery && right <= rightQuery)
		{
			// Update the lazy value
			lazy[node].value += val;
			// Apply the update immediately
			push(left, right, node);
			return;
		}
		// Recursively update the left child
		update(left, mid, L, leftQuery, rightQuery, val);
		// Recursively update the right child
		update(mid + 1, right, R, leftQuery, rightQuery, val);
		// Merge the children values
		seg[node] = merge(seg[L], seg[R]);
	}
	Node query(int left, int right, int node, int leftQuery, int rightQuery)
	{
		// Apply the pending updates if any
		push(left, right, node);
		// If the range is invalid, return a value that does NOT to affect other queries
		if (left > rightQuery || right < leftQuery)
			return 0;

		// If the range matches the segment
		if (left >= leftQuery && right <= rightQuery)
			return seg[node];

		return merge(query(left, mid, L, leftQuery, rightQuery), query(mid + 1, right, R, leftQuery, rightQuery));
	}

public:
	LazySegmentTree(const vector<ll> &arr)
	{
		size = 1;
		int n = arr.size();
		while (size < n)
			size <<= 1;
		seg = vector<Node>(2 * size, 0);
		lazy = vector<Node>(2 * size, 0);
		build(0, size - 1, 0, arr);
	}
	void update(int left, int right, const ll &val)
	{
		update(0, size - 1, 0, left, right, val);
	}
	ll query(int left, int right)
	{
		Node ans = query(0, size - 1, 0, left, right);
		return ans.value;
	}

#undef L
#undef R
#undef mid
};

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
	void erase(int L, int R)
	{
		auto [l, s, r] = SplayTree::parts(root, L, R);
		SplayTree::freeNodes(s), root = SplayTree::merge(l, r);
	}
	SplayTree *findKth(int k)
	{
		SplayTree *n = SplayTree::findKth(root, k);
		return n ? root = n : n;
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
		ll N, M;
		char c;
		cin >> N >> M;
		string str;
		cin >> str;

		map<char, set<int>> pos;
		vector<ll> vc(N);
		iota(vc.begin(), vc.end(), 1);
		LazySegmentTree segTree(vc);

		for (int i = 0; i < N; i++)
			pos[str[i]].insert(i + 1);
		Tree tree(vc);

		while (M--)
		{
			ll L, R;
			cin >> L >> R >> c;
			L = tree.findKth(L)->value;
			R = tree.findKth(R)->value;
			while (true)
			{
				auto it = pos[c].lower_bound(L);
				if (it == pos[c].end() || *it > R)
					break;
				int curPos = segTree.query(*it - 1, *it - 1);
				tree.erase(curPos, curPos);
				segTree.update((*it), N, -1);
				pos[c].erase(it);
			}
		}
		tree.inorder([&](ll v)
		             { cout << str[v - 1]; });
		cout << endl;
	}
	return 0;
}