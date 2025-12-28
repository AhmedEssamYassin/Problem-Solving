#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

// Flattening binary trees is useful to do subtree queries.

struct LazySegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		ll value;
		Node() { value = 0; }
		Node(const ll &N) : value(N) {}
	};
	struct LazyNode
	{
		ll value;
		LazyNode() { value = 0; }
		LazyNode(const ll &N) : value(N) {}
		LazyNode operator+=(const LazyNode &RHS)
		{
			value += RHS.value;
			return *this;
		}
	};
	int size;
	vector<Node> seg;
	vector<LazyNode> lazy;
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
			lazy[L] += lazy[node];
			// Update the lazy values for the right child
			lazy[R] += lazy[node];
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
			lazy[node] = val;
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
		seg = vector<Node>(2 * size, 0);
		lazy = vector<LazyNode>(2 * size, 0); // Initialize the LazyNode with a Not-possible value
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

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
	int t = 1;
	// cin >> t;
	while (t--)
	{
		ll n, m;
		cin >> n >> m;
		vector<ll> a(n + 1);
		for (int i{1}; i <= n; i++)
			cin >> a[i];
		vector<vector<ll>> Tree(n + 1);
		for (int i{}; i < n - 1; i++)
		{
			ll u, v;
			cin >> u >> v;
			Tree[u].push_back(v);
			Tree[v].push_back(u);
		}
		ll root = 1;
		int currTime = 0;
		vector<ll> in(n + 1), out(n + 1), depth(n + 1);
		function<void(int, int, int)> eulerTour = [&](int u, int prev, int d) -> void
		{
			in[u] = currTime++;
			depth[u] = d;
			for (const ll &v : Tree[u])
			{
				if (v != prev)
					eulerTour(v, u, d + 1);
			}
			out[u] = currTime - 1;
		};
		eulerTour(root, -1, 0);
		vector<ll> evenNodes(n, 0), oddNodes(n, 0);
		LazySegmentTree evenSegTree(evenNodes);
		LazySegmentTree oddSegTree(oddNodes);

		while (m--)
		{
			ll type, x, val;
			cin >> type;
			if (type == 1)
			{
				cin >> x >> val;
				if (depth[x] & 1)
				{
					oddSegTree.update(in[x], out[x], val);
					evenSegTree.update(in[x], out[x], -val);
				}
				else
				{
					evenSegTree.update(in[x], out[x], val);
					oddSegTree.update(in[x], out[x], -val);
				}
			}
			else
			{
				cin >> x;
				ll curr = a[x];
				if (depth[x] & 1)
					curr += oddSegTree.query(in[x], in[x]);
				else
					curr += evenSegTree.query(in[x], in[x]);

				cout << curr << endl;
			}
		}
	}
	return 0;
}