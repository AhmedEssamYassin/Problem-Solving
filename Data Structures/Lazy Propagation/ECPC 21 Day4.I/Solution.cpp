#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"
#define inf LLONG_MAX
#define neg_inf LLONG_MIN

struct LazySegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		ll mx;
		ll mn;
		Node()
		{
			mx = neg_inf;
			mn = inf;
		}
		Node(const ll &N, const ll &M) : mx(N), mn(M) {}
	};
	struct LazyNode
	{
		ll lazyAdd;
		ll lazyMult;
		LazyNode()
		{
			lazyAdd = 0;
			lazyMult = 1;
		}
		LazyNode operator+=(const LazyNode &RHS)
		{
			lazyAdd += RHS.lazyAdd;
			return *this;
		}
		LazyNode operator*=(const LazyNode &RHS)
		{
			lazyMult *= RHS.lazyMult;
			return *this;
		}
	};
	int size;
	vector<Node> seg;
	vector<LazyNode> lazy;
	Node merge(const Node &leftNode, const Node &rightNode)
	{
		Node res;
		res.mx = max(leftNode.mx, rightNode.mx);
		res.mn = min(leftNode.mn, rightNode.mn);
		return res;
	}
	void build(int left, int right, int node, const vector<ll> &arr)
	{
		// If the segment has only one element, leaf node
		if (left == right)
		{
			if (left < arr.size())
				seg[node] = {arr[left], arr[left]};
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
		// Propagate multiplication first, then addition
		if (lazy[node].lazyMult != 1)
		{
			/*
			if x >= 0:
			max(a * x, b * x, c * x, d * x, e * x) = x * max(a, b, c, d, e)
			min(a * x, b * x, c * x, d * x, e * x) = x * min(a, b, c, d, e)

			if x < 0:
			max(a * x, b * x, c * x, d * x, e * x) = x * min(a, b, c, d, e)
			min(a * x, b * x, c * x, d * x, e * x) = x * max(a, b, c, d, e)
			*/
			if (lazy[node].lazyMult >= 0)
			{
				seg[node].mx *= lazy[node].lazyMult;
				seg[node].mn *= lazy[node].lazyMult;
			}
			else
			{
				int oldMax = seg[node].mx;
				int oldMin = seg[node].mn;
				seg[node].mx = oldMin * lazy[node].lazyMult;
				seg[node].mn = oldMax * lazy[node].lazyMult;
			}

			// If the node is not a leaf
			if (left != right)
			{
				// Update the lazy values for the left child
				lazy[L].lazyMult *= lazy[node].lazyMult;
				lazy[L].lazyAdd *= lazy[node].lazyMult;
				// Update the lazy values for the right child
				lazy[R].lazyMult *= lazy[node].lazyMult;
				lazy[R].lazyAdd *= lazy[node].lazyMult;
			}
			// Reset the lazy value
			lazy[node].lazyMult = 1;
		}

		// Propagate the addition value
		if (lazy[node].lazyAdd != 0)
		{
			// max(a + x, b + x, c + x, d + x, e + x) = x + max(a, b, c, d, e)
			// min(a + x, b + x, c + x, d + x, e + x) = x + min(a, b, c, d, e)
			seg[node].mx += lazy[node].lazyAdd;
			seg[node].mn += lazy[node].lazyAdd;

			// If the node is not a leaf
			if (left != right)
			{
				// Update the lazy values for the left child
				lazy[L].lazyAdd += lazy[node].lazyAdd;
				// Update the lazy values for the right child
				lazy[R].lazyAdd += lazy[node].lazyAdd;
			}
			// Reset the lazy value
			lazy[node].lazyAdd = 0;
		}
	}
	void update(int left, int right, int node, int leftQuery, int rightQuery, int type, const ll &val)
	{
		push(left, right, node);
		// If the range is invalid, return
		if (left > rightQuery || right < leftQuery)
			return;
		// If the range matches the segment
		if (left >= leftQuery && right <= rightQuery)
		{
			// Update the lazy value
			if (type == 1)
				lazy[node].lazyMult = val;
			else
				lazy[node].lazyAdd = val;
			// Apply the update immediately
			push(left, right, node);
			return;
		}
		// Recursively update the left child
		update(left, mid, L, leftQuery, rightQuery, type, val);
		// Recursively update the right child
		update(mid + 1, right, R, leftQuery, rightQuery, type, val);
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
	void update(int left, int right, int type, const ll &val)
	{
		update(0, size - 1, 0, left, right, type, val);
	}
	ll query(int left, int right)
	{
		Node ans = query(0, size - 1, 0, left, right);
		return ans.mx;
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
	freopen("rmq.in", "r", stdin);
	int t = 1;
	ll N, q;
	cin >> t;
	while (t--)
	{
		cin >> N >> q;
		vector<ll> vc(N);
		for (int i{}; i < N; i++)
			cin >> vc[i];
		LazySegmentTree segTree(vc);
		while (q--)
		{
			ll type, l, r;
			cin >> type >> l >> r;
			l--, r--;
			const ll &mx = segTree.query(l, r);
			if (type == 1)
			{
				segTree.update(l, r, 1, -1);
				segTree.update(l, r, 2, mx);
			}
			else
				cout << segTree.query(l, r) << endl;
		}
	}
	return 0;
}