#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

constexpr int BITS = 20;
struct XORBasis
{
	using word = conditional_t<(BITS > 32), uint64_t, uint32_t>;
	array<word, BITS> basis{}; // basis[i] is 0 or has its highest bit at i
	int sz = 0;

	XORBasis(word x = 0) { insertVector(x); }

	bool insertVector(word x) // true if x was independent of the basis
	{
		for (int i = 0; x && sz < BITS; x ^= basis[i])
			if (!basis[i = __lg(x)])
				return basis[i] = x, sz++, true;
		return false;
	}

	bool canRepresent(word x) const
	{
		for (int i = 0; x; x ^= basis[i])
			if (!basis[i = __lg(x)])
				return 0;
		return 1;
	}

	word getMaxXor() const
	{
		word r = 0;
		for (int i = BITS; i--;)
			r = max(r, r ^ basis[i]);
		return r;
	}

	XORBasis &operator+=(const XORBasis &o)
	{
		if (o.sz == BITS) // Full basis spans everything
			return *this = o;
		for (word v : o.basis)
			insertVector(v);
		return *this;
	}
	friend XORBasis operator+(XORBasis a, const XORBasis &b) { return a += b; }

	void clear() { *this = {}; }
};

struct LazySegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		ll value;
		XORBasis xB;
		Node() {}
		Node(const ll &N)
		{
			value = N;
			xB.insertVector(N);
		}
	};
	struct LazyNode
	{
		ll value;
		LazyNode() {}
		LazyNode(const ll &N) : value(N) {}
		LazyNode operator&(const LazyNode &RHS)
		{
			value = (value & RHS.value);
			return *this;
		}
	};
	int size;
	vector<Node> seg;
	vector<LazyNode> lazy;
	Node merge(const Node leftNode, const Node &rightNode)
	{
		Node res;
		res.xB = (leftNode.xB + rightNode.xB);
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
		if (lazy[node].value != -1)
		{
			// Propagate the value
			ll x = lazy[node].value;
			XORBasis &A = seg[node].xB;
			auto B = A.basis; // A copy
			A.clear();
			for (const int &y : B)
				A.insertVector(x & y);
			// If the node is not a leaf
			if (left != right)
			{
				// Update the lazy values for the left child
				lazy[L] = (lazy[L] & lazy[node]);
				// Update the lazy values for the right child
				lazy[R] = (lazy[R] & lazy[node]);
			}
			// Reset the lazy value
			lazy[node] = -1;
		}
	}
	void rangeUpdate(int left, int right, int node, int leftQuery, int rightQuery, const ll &val)
	{
		push(left, right, node);
		// If the range is invalid, return
		if (left > rightQuery || right < leftQuery)
			return;
		// If the range matches the segment
		if (left >= leftQuery && right <= rightQuery)
		{
			// Update the lazy value
			lazy[node] = (lazy[node] & val);

			// Apply the update immediately
			push(left, right, node);
			return;
		}
		// Recursively update the left child
		rangeUpdate(left, mid, L, leftQuery, rightQuery, val);
		// Recursively update the right child
		rangeUpdate(mid + 1, right, R, leftQuery, rightQuery, val);
		// Merge the children values
		seg[node] = merge(seg[L], seg[R]);
	}

	void pointUpdate(int left, int right, int node, int idx, const ll &val)
	{
		push(left, right, node);
		if (idx > right || idx < left)
			return;
		if (left == right)
		{
			seg[node].xB.clear();
			seg[node] = val;
			return;
		}
		pointUpdate(left, mid, L, idx, val);
		pointUpdate(mid + 1, right, R, idx, val);
		// Updating while returning to parent nodes
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
		lazy = vector<LazyNode>(2 * size, -1); // 111111111111...
		build(0, size - 1, 0, arr);
	}
	void update(int left, int right, const ll &val, int type, int idx = 0)
	{
		if (type == 1)
			rangeUpdate(0, size - 1, 0, left, right, val);
		else
			pointUpdate(0, size - 1, 0, idx, val);
	}
	ll query(int left, int right)
	{
		Node ans = query(0, size - 1, 0, left, right);
		return ans.xB.getMaxXor();
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
	ll N, M;
	cin >> t;
	while (t--)
	{
		cin >> N >> M;
		vector<ll> A(N);
		for (int i{}; i < N; i++)
			cin >> A[i];
		LazySegmentTree segTree(A);

		while (M--)
		{
			ll query, L, R, x, i, v;
			cin >> query;
			if (query == 1)
			{
				cin >> L >> R >> x;
				L--, R--;
				segTree.update(L, R, x, 1);
			}
			else if (query == 2)
			{
				cin >> i >> x;
				i--;
				segTree.update(0, 0, x, 2, i);
			}
			else
			{
				cin >> L >> R;
				L--, R--;
				cout << segTree.query(L, R) << endl;
			}
		}
	}
	return 0;
}