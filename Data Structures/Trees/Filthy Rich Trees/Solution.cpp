#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

// Flattening binary trees is useful to do subtree queries.

struct SegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		long double logSum;
		// Constructors
		Node() {}
		Node(long double val) : logSum(val) {}
	};
	int size;
	vector<Node> seg;
	Node merge(const Node &leftNode, const Node &rightNode)
	{
		Node res;
		res.logSum = (leftNode.logSum + rightNode.logSum);
		return res;
	}
	void build(int left, int right, int node, const vector<ll> &arr)
	{
		if (left == right) // Leaf Node (single element)
		{
			if (left < arr.size()) // Making sure we are inside the boundaries of the array
				seg[node] = arr[left];
			return;
		}
		// Building left node
		build(left, mid, L, arr);

		// Building right node
		build(mid + 1, right, R, arr);

		// Returning to parent nodes
		seg[node] = merge(seg[L], seg[R]);
	}
	void update(int left, int right, int node, int idx, const long double &val)
	{
		if (left == right)
		{
			seg[node] = val;
			return;
		}
		if (idx <= mid)
			update(left, mid, L, idx, val);
		else
			update(mid + 1, right, R, idx, val);
		// Updating while returning to parent nodes
		seg[node] = merge(seg[L], seg[R]);
	}
	Node query(int left, int right, int node, int leftQuery, int rightQuery)
	{
		// Out of range
		if (right < leftQuery || left > rightQuery)
			return 0; // A value that doesn't affect the query

		// The whole range is the answer
		if (left >= leftQuery && right <= rightQuery)
			return seg[node];

		// ONLY a part of this segment belongs to the query
		Node leftSegment = query(left, mid, L, leftQuery, rightQuery);
		Node rightSegment = query(mid + 1, right, R, leftQuery, rightQuery);
		return merge(leftSegment, rightSegment);
	}

public:
	SegmentTree(const vector<ll> &arr)
	{
		size = 1;
		int n = arr.size();
		while (size < n)
			size <<= 1;
		seg = vector<Node>(2 * size, 0);
		build(0, size - 1, 0, arr);
	}
	void update(int idx, const long double &val)
	{
		update(0, size - 1, 0, idx, val);
	}
	long double query(int left, int right)
	{
		Node ans = query(0, size - 1, 0, left, right);
		return ans.logSum;
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
	ll N;
	// cin >> t;
	while (t--)
	{
		cin >> N;
		vector<vector<ll>> Tree(N + 1);
		int anyNode = 1;
		for (int i{}; i < N - 1; i++)
		{
			ll u, v;
			cin >> u >> v;
			anyNode = u;
			// Tree edges are indirected
			Tree[u].push_back(v);
			Tree[v].push_back(u);
		}
		int root = 1;
		// If the tree is not rooted
		// root = anyNode;
		int currTime = 0;
		vector<ll> in(N + 1), out(N + 1);
		function<void(int, int)> eulerTour = [&](int u, int prev) -> void
		{
			in[u] = currTime++;
			for (const ll &v : Tree[u])
			{
				if (v != prev)
					eulerTour(v, u);
			}
			out[u] = currTime - 1;
		};
		eulerTour(root, -1); // root is stated to be 1
		vector<ll> nodes(N);
		for (int u = 1; u <= N; u++)
			nodes[in[u]] = 0; // They are initially 1's, and log(1) = 0

		SegmentTree segTree(nodes);
		/*
		NOTE: log() in C++ is ln() in math
		----------------------------------
		a * b * c ... behaves like log(a * b * c) in comparison
		log(a * b * c) = log(a) + log(b) + log(c) ...
		(a / b) = e ^ (log(a) - log(b))
		*/
		const long double e = 2.71828182845904523536;
		cout << fixed << setprecision(10);
		ll Q;
		cin >> Q;
		while (Q--)
		{
			ll type, x, y;
			cin >> type >> x >> y;
			if (type == 1)
				segTree.update(in[x], logl(y));
			else
			{
				long double logSumSubtreeX = segTree.query(in[x], out[x]);
				long double logSumSubtreeY = segTree.query(in[y], out[y]);
				if (logSumSubtreeX - logSumSubtreeY >= logl(1e9))
					cout << 1000000000 << endl;
				else
					cout << powl(e, logSumSubtreeX - logSumSubtreeY) << endl;
			}
		}
	}
	return 0;
}