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
		ll sum;
		ll freq[31]{};
		Node() {}
		Node(const ll &N)
		{
			sum = N;
			for (int i{}, cur = N; i < 31 && cur > 0; i++, cur >>= 1)
				freq[i] = (cur & 1);
		}
	};
	struct LazyNode
	{
		ll value;
		LazyNode() {}
		LazyNode(const ll &N) : value(N) {}
		LazyNode operator^(const LazyNode &RHS)
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
		res.sum = (leftNode.sum + rightNode.sum);
		for (int i{}; i < 31; i++)
			res.freq[i] = (leftNode.freq[i] + rightNode.freq[i]);
		return res;
	}
	void build(int left, int right, int node, const vector<ll> &arr)
	{
		// Initialize the lazy array
		lazy[node] = 0;
		// If the segment has only one element
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
		if (lazy[node].value != 0)
		{
			ll val{}, lazyVal = lazy[node].value;
			for (int i{}; i < 31; i++, lazyVal >>= 1)
			{
				if (lazyVal & 1) // Flips all bits in freq
					seg[node].freq[i] = (right - left + 1) - seg[node].freq[i];
				val += seg[node].freq[i] * (1LL << i);
			}
			seg[node].sum = val;
			// If the node is not a leaf
			if (left != right)
			{
				// Update the lazy values for the left child
				lazy[L] = (lazy[L] ^ lazy[node]);
				// Update the lazy values for the right child
				lazy[R] = (lazy[R] ^ lazy[node]);
			}
			// Reset the lazy value
			lazy[node] = 0;
		}
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
			lazy[node] = (lazy[node] ^ val);

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
		// If the range is invalid, return set bits NOT to affect other queries
		if (left > rightQuery || right < leftQuery)
			return 0;

		// If the range matches the segment
		if (left >= leftQuery && right <= rightQuery)
			return seg[node];
		Node getLeftQuery = query(left, mid, L, leftQuery, rightQuery);
		Node getRightQuery = query(mid + 1, right, R, leftQuery, rightQuery);
		return merge(getLeftQuery, getRightQuery);
	}

public:
	LazySegmentTree(const vector<ll> &arr)
	{
		size = 1;
		int n = arr.size();
		while (size < n)
			size <<= 1;

		seg = vector<Node>(2 * size, 0);
		lazy = vector<LazyNode>(2 * size, 0);
		build(0, size - 1, 0, arr);
	}
	void update(int left, int right, const ll &val)
	{
		update(0, size - 1, 0, left, right, val);
	}
	ll query(int left, int right)
	{
		Node ans = query(0, size - 1, 0, left, right);
		return ans.sum;
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
	ll N, Q;
	// cin >> t;
	while (t--)
	{
		cin >> N;
		vector<ll> vc(N);
		for (int i{}; i < N; i++)
			cin >> vc[i];
		LazySegmentTree segTree(vc);
		cin >> Q;
		while (Q--)
		{
			ll query, L, R, x;
			cin >> query >> L >> R;
			L--, R--;
			if (query == 1)
				cout << segTree.query(L, R) << endl;
			else
			{
				cin >> x;
				segTree.update(L, R, x);
			}
		}
	}
	return 0;
}