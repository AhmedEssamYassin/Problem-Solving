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
		ll c4, c7, lis, lds;
		Node() { c4 = c7 = lis = lds = 0; }
		Node(const ll &N)
		{
			if (N == 4)
				c4 = 1, c7 = 0, lis = 1, lds = 1;
			else
				c4 = 0, c7 = 1, lis = 1, lds = 1;
		}
	};
	struct LazyNode
	{
		ll flip;
		LazyNode() { flip = false; }
		LazyNode(const ll &N) : flip(N) {}
		LazyNode operator^=(const LazyNode &RHS)
		{
			flip ^= RHS.flip;
			return *this;
		}
	};
	int size;
	vector<Node> seg;
	vector<LazyNode> lazy;
	Node merge(const Node &leftNode, const Node &rightNode)
	{
		Node res;
		res.c4 = (leftNode.c4 + rightNode.c4);
		res.c7 = (leftNode.c7 + rightNode.c7);
		res.lis = max(leftNode.c4 + rightNode.lis, leftNode.lis + rightNode.c7);
		res.lds = max(leftNode.c7 + rightNode.lds, leftNode.lds + rightNode.c4);
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
		if (lazy[node].flip == 0)
			return;
		swap(seg[node].c4, seg[node].c7);
		swap(seg[node].lis, seg[node].lds);
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
			lazy[node].flip ^= val;
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
			return Node();

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
		seg = vector<Node>(2 * size);
		lazy = vector<LazyNode>(2 * size);
		build(0, size - 1, 0, arr);
	}
	void update(int left, int right, const ll &val)
	{
		update(0, size - 1, 0, left, right, val);
	}
	ll query(int left, int right)
	{
		Node ans = query(0, size - 1, 0, left, right);
		return ans.lis;
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
	// cin >> t;
	while (t--)
	{
		ll n, m;
		cin >> n >> m;
		string str;
		cin >> str;
		vector<ll> vc(n);
		for (int i{}; i < n; i++)
			vc[i] = str[i] - '0';
		LazySegmentTree segTree(vc);
		while (m--)
		{
			string type;
			ll l, r;
			cin >> type;
			if (type == "switch")
			{
				cin >> l >> r;
				--l, --r;
				segTree.update(l, r, 1);
			}
			else
				cout << segTree.query(0, n - 1) << endl;
		}
	}
	return 0;
}