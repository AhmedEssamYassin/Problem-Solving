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
		ll cntZ;
		Node() { value = 0, cntZ = 0; }
		Node(const ll &N) { value = N, cntZ = (N == 0); }
	};
	struct LazyNode
	{
		ll value;
		LazyNode() { value = -1; }
		LazyNode(const ll &N) : value(N) {}
		LazyNode operator=(const LazyNode &RHS)
		{
			value = RHS.value;
			return *this;
		}
	};
	int size;
	vector<Node> seg;
	vector<LazyNode> lazy;
	Node merge(const Node &leftNode, const Node &rightNode)
	{
		Node res;
		res.value = (leftNode.value + rightNode.value);
		res.cntZ = (leftNode.cntZ + rightNode.cntZ);
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
		if (lazy[node].value == -1)
			return;
		seg[node].value = (right - left + 1) * lazy[node].value;
		seg[node].cntZ = (lazy[node].value == 0) ? (right - left + 1) : 0;
		// If the node is not a leaf
		if (left != right)
		{
			// Update the lazy values for the left child
			lazy[L] = lazy[node];
			// Update the lazy values for the right child
			lazy[R] = lazy[node];
		}
		// Reset the lazy value
		lazy[node] = LazyNode();
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
		seg = vector<Node>(2 * size);
		lazy = vector<LazyNode>(2 * size); // Initialize the LazyNode with a Not-possible value
		build(0, size - 1, 0, arr);
	}
	void update(int left, int right, const ll &val)
	{
		update(0, size - 1, 0, left, right, val);
	}
	ll query(int left, int right)
	{
		Node ans = query(0, size - 1, 0, left, right);
		return ans.cntZ;
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
		int N;
		cin >> N;
		vector<vector<ll>> Tree(N + 1);
		for (int i{}; i < N - 1; i++)
		{
			ll u, v;
			cin >> u >> v;
			Tree[u].push_back(v);
			Tree[v].push_back(u);
		}
		ll root = 1;
		int currTime = 0;
		vector<ll> in(N + 1), out(N + 1), parent(N + 1);
		function<void(int, int)> eulerTour = [&](int u, int prev) -> void
		{
			in[u] = currTime++;
			for (const ll &v : Tree[u])
			{
				if (v != prev)
					eulerTour(v, u);
				parent[v] = u;
			}
			out[u] = currTime - 1;
		};
		eulerTour(root, -1);
		vector<ll> nodes(N, 0);
		LazySegmentTree segTree(nodes);
		ll Q;
		cin >> Q;
		while (Q--)
		{
			ll type, u;
			cin >> type >> u;
			if (type == 1)
			{
				if (segTree.query(in[u], out[u]))
					segTree.update(in[parent[u]], in[parent[u]], 0); // Push it up, to maintain the 0 for ancestors
				segTree.update(in[u], out[u], 1);
			}
			else if (type == 2)
				segTree.update(in[u], in[u], 0);
			else
				cout << !segTree.query(in[u], out[u]) << endl;
		}
	}
	return 0;
}