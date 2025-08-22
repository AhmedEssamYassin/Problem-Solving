#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

const ll mod = 1e9 + 7;
#define double_size_t std::conditional_t<(mod > (1LL << 31)), __int128_t, long long>
inline ll add64(const ll &a, const ll &b)
{
	double_size_t res = double_size_t(a) + b;
	if (res >= mod)
		res -= mod;
	return res;
}

inline ll sub64(const ll &a, const ll &b)
{
	double_size_t res = double_size_t(a) - b;
	if (res < 0)
		res += mod;
	if (res >= mod)
		res -= mod;
	return res;
}

inline ll mult64(const ll &a, const ll &b)
{
	return double_size_t(a) * b % mod;
}

// Iterative Fast Doubling method
pair<ll, ll> fastDoubling(ll n)
{
	ll a = 0, b = 1; // (F(0), F(1))
	if (n == 0)
		return {a, b};

	int msb = __lg(n);
	for (int i = msb; i >= 0; i--)
	{
		ll c = mult64(a, sub64(mult64(b, 2), a)); // F(2k)
		ll d = add64(mult64(a, a), mult64(b, b)); // F(2k + 1)
		if ((n >> i) & 1)
		{
			a = d;
			b = add64(c, d);
		}
		else
		{
			a = c;
			b = d;
		}
	}
	return {a, b};
}

pair<ll, ll> F(ll N)
{
	if (N < 0)
		return {0, 0};
	return fastDoubling(N); // F(N), F(N + 1)
}

struct LazySegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		ll S;
		ll prevS;
		Node() { prevS = 0, S = 0; }
		Node(const pair<ll, ll> &P) : prevS(P.first), S(P.second) {}
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
		Node res;
		res.prevS = add64(leftNode.prevS, rightNode.prevS);
		res.S = add64(leftNode.S, rightNode.S);
		return res;
	}
	void build(int left, int right, int node, const vector<ll> &arr)
	{
		// If the segment has only one element, leaf node
		if (left == right)
		{
			if (left < arr.size())
				seg[node] = F(arr[left] - 1);
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
		const ll &k = lazy[node].value;
		const auto &[Fk, Fk_plus_1] = F(k);
		ll newSumFib = add64(mult64(Fk_plus_1, seg[node].S), mult64(Fk, seg[node].prevS));
		ll newSumFibPrev = add64(mult64(Fk, seg[node].S), mult64(F(k - 1).first, seg[node].prevS));

		seg[node].S = newSumFib;
		seg[node].prevS = newSumFibPrev;
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
		return ans.S;
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
		int n, m;
		cin >> n >> m;
		vector<ll> vc(n);
		for (int i{}; i < n; i++)
			cin >> vc[i];
		LazySegmentTree segTree(vc);
		while (m--)
		{
			int tp, l, r;
			ll x;
			cin >> tp >> l >> r;
			--l, --r;
			if (tp == 1)
			{
				cin >> x;
				segTree.update(l, r, x);
			}
			else
				cout << segTree.query(l, r) << endl;
		}
	}
	return 0;
}