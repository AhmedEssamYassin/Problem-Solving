#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

using u64 = uint64_t;
u64 S = chrono::steady_clock::now().time_since_epoch().count() ^ (u64) new char;
u64 rnd()
{
	S += 0xa0761d6478bd642f;
	__uint128_t t = (__uint128_t)S * (S ^ 0xe7037ed1a0b428db);
	return t >> 64 ^ t;
}
#define rng(l, r) ((l) + (int64_t)((__uint128_t)rnd() * ((r) - (l) + 1) >> 64))

const u64 mod = (1LL << 61) - 1;
inline u64 add64(u64 a, u64 b)
{
	a += b;
	return a >= mod ? a - mod : a;
}
inline u64 sub64(u64 a, u64 b) { return a >= b ? a - b : a + mod - b; }
inline u64 mult64(u64 a, u64 b)
{
	__uint128_t t = (__uint128_t)a * b;
	u64 r = u64(t & mod) + u64(t >> 61);
	return r >= mod ? r - mod : r;
}
u64 modPow(u64 n, u64 e)
{
	u64 r = 1;
	for (; e; e >>= 1, n = mult64(n, n))
		if (e & 1)
			r = mult64(r, n);
	return r;
}

// Collision prob ~ n^2 / 2^122 per comparison.
struct H2
{
	u64 a, b;
	H2 operator+(H2 o) const { return {add64(a, o.a), add64(b, o.b)}; }
	H2 operator-(H2 o) const { return {sub64(a, o.a), sub64(b, o.b)}; }
	H2 operator*(H2 o) const { return {mult64(a, o.a), mult64(b, o.b)}; }
	H2 operator+(u64 x) const { return {add64(a, x), add64(b, x)}; }
	H2 operator-(u64 x) const { return {sub64(a, x), sub64(b, x)}; }
	H2 operator*(u64 x) const { return {mult64(a, x), mult64(b, x)}; }
	auto operator<=>(const H2 &) const = default;
};
const H2 B = {(u64)rng(1 << 20, mod - 2), (u64)rng(1 << 20, mod - 2)};
const H2 BI = {modPow(B.a, mod - 2), modPow(B.b, mod - 2)};
auto [Pb, sumB] = [](int mx)
{
	vector<H2> p(mx + 1, {1, 1}), s(mx + 1, {1, 1});
	for (int i = 1; i <= mx; i++)
	{
		p[i] = p[i - 1] * B;
		s[i] = s[i - 1] + p[i];
	}
	return pair{move(p), move(s)};
}(1e5);

struct Hash
{
	H2 code = {0, 0};
	int size = 0;
	Hash() = default;
	Hash(H2 c, int s) : code(c), size(s) {}
	Hash(ll x) : size(1)
	{
		u64 xm = ((x % (ll)mod) + mod) % mod;
		code = {xm, xm};
	}
	Hash(string_view s) : size(s.size())
	{
		for (uint8_t c : s)
			code = code * B + c;
	}
	void clear() { code = {0, 0}, size = 0; }

	Hash operator+(const Hash &o) const { return {code * Pb[o.size] + o.code, size + o.size}; }
	auto operator<=>(const Hash &) const = default;
};
// Rabin-Karp
struct HashRange
{
	vector<H2> p, s;
	HashRange() = default;
	HashRange(string_view t) : p(t.size() + 1, {0, 0}), s(t.size() + 1, {0, 0})
	{
		int n = t.size();
		for (int i = 0; i < n; i++)
		{
			p[i + 1] = p[i] * B + (uint8_t)t[i];
			s[n - 1 - i] = s[n - i] * B + (uint8_t)t[n - 1 - i];
		}
	}

	Hash get(int l, int r) const { return l <= r ? Hash{p[r + 1] - p[l] * Pb[r - l + 1], r - l + 1} : Hash{}; }
	Hash inv(int l, int r) const { return l <= r ? Hash{s[l] - s[r + 1] * Pb[r - l + 1], r - l + 1} : Hash{}; }
};

struct LazySegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		Hash h;
		Node() {}
		Node(const ll &N) : h(N) {}
	};
	struct LazyNode
	{
		ll lazyValue;
		LazyNode(const ll &val) : lazyValue(val) {}
	};
	int size;
	vector<Node> seg;
	vector<LazyNode> lazy;
	Node merge(const Node &leftNode, const Node &rightNode)
	{
		Node res;
		res.h = (leftNode.h + rightNode.h);
		return res;
	}
	void build(int left, int right, int node, const string &str)
	{
		// If the segment has only one element, leaf node
		if (left == right)
		{
			if (left < str.size())
				seg[node] = str[left];

			return;
		}
		// Recursively build the left child
		build(left, mid, L, str);
		// Recursively build the right child
		build(mid + 1, right, R, str);
		// Merge the children values
		seg[node] = merge(seg[L], seg[R]);
	}
	void push(int left, int right, int node)
	{
		// Propagate the value
		if (lazy[node].lazyValue != -1)
		{
			seg[node].h = {sumB[right - left] * (u64)lazy[node].lazyValue, right - left + 1};
			// If the node is not a leaf
			if (left != right)
			{
				// Update the lazy values for the left child
				lazy[L] = lazy[node];
				// Update the lazy values for the right child
				lazy[R] = lazy[node];
			}
			// Reset the lazy value
			lazy[node] = -1;
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
		Node leftSegment = query(left, mid, L, leftQuery, rightQuery);
		Node rightSegment = query(mid + 1, right, R, leftQuery, rightQuery);
		return merge(leftSegment, rightSegment);
	}

public:
	LazySegmentTree(const string &str)
	{
		size = 1;
		int n = str.size();
		while (size < n)
			size <<= 1;
		seg = vector<Node>(2 * size);
		lazy = vector<LazyNode>(2 * size, -1);
		build(0, size - 1, 0, str);
	}
	void update(int left, int right, const ll &val)
	{
		update(0, size - 1, 0, left, right, val);
	}
	Hash query(int left, int right)
	{
		Node ans = query(0, size - 1, 0, left, right);
		return ans.h;
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
	ll N, m, k;
	string str;
	// cin >> t;
	while (t--)
	{
		cin >> N >> m >> k >> str;
		m += k;
		LazySegmentTree segTree(str);
		while (m--)
		{
			ll t, l, r, c;
			cin >> t >> l >> r >> c;
			if (t == 1)
				segTree.update(--l, --r, c + 48);
			else
				cout << (segTree.query(l - 1, r - c - 1) == segTree.query(l + c - 1, r - 1) ? "YES\n" : "NO\n");
		}
	}
	return 0;
}