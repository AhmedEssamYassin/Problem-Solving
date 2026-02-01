#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

const ll mod = 1e9 + 9;
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

ll modPow(ll N, ll power)
{
	if (N % mod == 0 || N == 0)
		return 0;
	if (N == 1 || power == 0)
		return 1;

	ll res{1};
	while (power)
	{
		if (power & 1)
			res = mult64(res, N);
		N = mult64(N, N);
		power >>= 1;
	}
	return res;
}

vector<ll> F;
void precompute(int N)
{
	F.resize(N + 4);
	F[0] = 0;
	F[1] = 1;
	F[2] = 1;
	for (int i = 3; i <= N + 3; i++)
		F[i] = add64(F[i - 1], F[i - 2]);
}
static int autoCall = (precompute(3e5), 0);

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
		ll a, b;
		LazyNode() { a = 0, b = 0; }
		LazyNode(const ll &a, const ll &b) : a(a), b(b) {}
	};
	int size;
	vector<Node> seg;
	vector<LazyNode> lazy;
	Node merge(const Node &leftNode, const Node &rightNode)
	{
		Node res = add64(leftNode.value, rightNode.value);
		return res;
	}

	ll getFibSum(ll a, ll b, int k)
	{
		if (k <= 0)
			return 0;
		/*
		G[0] = a
		G[1] = b
		G[2] = a + b
		G[3] = a + 2 * b
		G[4] = 2 * a + 3 * b
		G[n] = a * F[n - 1] + b * F[n]

		Sum of the first k terms (from G[0] to G[k - 1]) = G[k + 1] - G[1]
		= a * F[k + 1 - 1] + b * F[k + 1] - b
		= a * F[k] + b * F[k + 1] - b
		The sum = a * F[k] + b * (F[k + 1] - 1)
		*/
		ll term1 = mult64(a, F[k]);
		ll term2 = mult64(b, sub64(F[k + 1], 1));
		return add64(term1, term2);
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
		if (lazy[node].a == 0 && lazy[node].b == 0)
			return;
		const ll &da = lazy[node].a;
		const ll &db = lazy[node].b;
		seg[node].value = add64(seg[node].value, getFibSum(da, db, right - left + 1));
		// If the node is not a leaf
		if (left != right)
		{
			int lenL = mid - left + 1;
			int lenR = right - (mid + 1) + 1;

			// Propagate to left child
			lazy[L].a = add64(lazy[L].a, da);
			lazy[L].b = add64(lazy[L].b, db);

			// Propagate to right child

			/*
			G[n] = a * F[n - 1] + b * F[n]
			The sequence starts at term (lenL)
			G[lenL] = a * F[lenL - 1] + b * F[lenL]
			G[lenL + 1] = a * F[lenL] + b * F[lenL + 1]
			*/
			ll new_a = add64(mult64(da, lenL > 0 ? F[lenL - 1] : 1), mult64(db, F[lenL]));
			ll new_b = add64(mult64(da, F[lenL]), mult64(db, F[lenL + 1]));
			lazy[R].a = add64(lazy[R].a, new_a);
			lazy[R].b = add64(lazy[R].b, new_b);
		}

		lazy[node] = {0, 0};
	}
	void update(int left, int right, int node, int leftQuery, int rightQuery)
	{
		push(left, right, node);
		// If the range is invalid, return
		if (left > rightQuery || right < leftQuery)
			return;
		// If the range matches the segment
		if (left >= leftQuery && right <= rightQuery)
		{
			// Update the lazy value
			int k = left - leftQuery + 1;
			ll da = F[k];
			ll db = F[k + 1];
			lazy[node].a = add64(lazy[node].a, da);
			lazy[node].b = add64(lazy[node].b, db);
			push(left, right, node);
			return;
		}
		// Recursively update the left child
		update(left, mid, L, leftQuery, rightQuery);
		// Recursively update the right child
		update(mid + 1, right, R, leftQuery, rightQuery);
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
	void update(int left, int right)
	{
		update(0, size - 1, 0, left, right);
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
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif
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
			int type;
			int L, R;
			cin >> type >> L >> R;
			L--, R--; // 0-indexed
			if (type == 1)
				segTree.update(L, R);
			else
				cout << segTree.query(L, R) << endl;
		}
	}
	return 0;
}