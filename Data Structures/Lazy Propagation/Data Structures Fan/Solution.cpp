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
		ll v0, v1, s;
		Node() { s = 0, v0 = 0, v1 = 0; }
		Node(const ll &z, const ll &o, const ll &si)
		{
			v0 ^= z;
			v1 ^= o;
			s = si;
		}
	};
	struct LazyNode
	{
		ll value;
		LazyNode() { value = 0; }
		LazyNode(const ll &N) : value(N) {}
		LazyNode operator^=(const LazyNode &RHS)
		{
			value ^= RHS.value;
			return *this;
		}
	};
	int size;
	vector<Node> seg;
	vector<LazyNode> lazy;
	Node merge(const Node &leftNode, const Node &rightNode)
	{
		Node res;
		res.v0 = (leftNode.v0 ^ rightNode.v0);
		res.v1 = (leftNode.v1 ^ rightNode.v1);
		res.s = (leftNode.s + rightNode.s);
		return res;
	}
	void build(int left, int right, int node, const vector<pair<ll, ll>> &arr)
	{
		// If the segment has only one element, leaf node
		if (left == right)
		{
			if (left < arr.size())
			{
				const auto &[ai, si] = arr[left];
				seg[node] = Node(ai * (si == 0), ai * (si == 1), si);
			}
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
		seg[node].s = (right - left + 1) - seg[node].s;
		swap(seg[node].v0, seg[node].v1);
		// If the node is not a leaf
		if (left != right)
		{
			// Update the lazy values for the left child
			lazy[L] ^= lazy[node];
			// Update the lazy values for the right child
			lazy[R] ^= lazy[node];
		}
		// Reset the lazy value
		lazy[node] = 0;
	}
	void update(int left, int right, int node, int leftQuery, int rightQuery, const ll &x)
	{
		push(left, right, node);
		// If the range is invalid, return
		if (left > rightQuery || right < leftQuery)
			return;
		// If the range matches the segment
		if (left >= leftQuery && right <= rightQuery)
		{
			// Update the lazy value
			lazy[node].value = x;
			// Apply the update immediately
			push(left, right, node);
			return;
		}
		// Recursively update the left child
		update(left, mid, L, leftQuery, rightQuery, x);
		// Recursively update the right child
		update(mid + 1, right, R, leftQuery, rightQuery, x);
		// Merge the children values
		seg[node] = merge(seg[L], seg[R]);
	}
	Node query(int left, int right, int node, int leftQuery, int rightQuery)
	{
		// Apply the pending updates if any
		push(left, right, node);
		// If the range is invalid, return a value that does NOT to affect other queries
		if (left > rightQuery || right < leftQuery)
			return Node();

		// If the range matches the segment
		if (left >= leftQuery && right <= rightQuery)
			return seg[node];

		return merge(query(left, mid, L, leftQuery, rightQuery), query(mid + 1, right, R, leftQuery, rightQuery));
	}

public:
	LazySegmentTree(const vector<pair<ll, ll>> &arr)
	{
		size = 1;
		int n = arr.size();
		while (size < n)
			size <<= 1;
		seg = vector<Node>(2 * size);
		lazy = vector<LazyNode>(2 * size);
		build(0, size - 1, 0, arr);
	}
	void update(int left, int right, const ll &val)
	{
		update(0, size - 1, 0, left, right, val);
	}
	pair<ll, ll> query(int left, int right)
	{
		Node ans = query(0, size - 1, 0, left, right);
		return {ans.v0, ans.v1};
	}

#undef L
#undef R
#undef mid
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
	cin >> t;
	while (t--)
	{
		int n;
		cin >> n;
		vector<ll> a(n);
		string str;
		for (int i{}; i < n; i++)
			cin >> a[i];
		cin >> str;
		vector<pair<ll, ll>> vc;
		for (int i{}; i < n; i++)
			vc.emplace_back(a[i], str[i] - '0');
		LazySegmentTree segTree(vc);
		int q;
		cin >> q;
		while (q--)
		{
			int tp, g, l, r;
			cin >> tp;
			if (tp == 1)
			{
				cin >> l >> r;
				--l, --r; // To be 0-based
				segTree.update(l, r, 1);
			}
			else
			{
				cin >> g;
				const auto &[v0, v1] = segTree.query(0, n - 1);
				cout << (g ? v1 : v0) << " ";
			}
		}
		cout << endl;
	}
	return 0;
}