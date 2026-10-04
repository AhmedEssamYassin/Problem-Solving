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
}(1e6);

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

struct SegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		Hash forwardHash;
		Hash backwardHash;
		// Constructors
		Node() {}
		Node(const ll &val) : forwardHash(val), backwardHash(val) {}
	};
	int size;
	vector<Node> seg;
	Node merge(const Node &leftNode, const Node &rightNode)
	{
		Node res;
		res.forwardHash = (leftNode.forwardHash + rightNode.forwardHash);
		res.backwardHash = (rightNode.backwardHash + leftNode.backwardHash);
		return res;
	}
	void build(int left, int right, int node, const string &arr)
	{
		if (left == right) // Leaf Node (single element)
		{
			if (left < arr.size()) // Making sure we are inside the boundaries of the array
				seg[node] = arr[left] - 'a';
			return;
		}
		// Building left node
		build(left, mid, L, arr);

		// Building right node
		build(mid + 1, right, R, arr);

		// Returning to parent nodes
		seg[node] = merge(seg[L], seg[R]);
	}
	void update(int left, int right, int node, int idx, const ll &val)
	{
		if (left == right)
		{
			seg[node] = val - 'a';
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
			return Node(); // A value that doesn't affect the query

		// The whole range is the answer
		if (left >= leftQuery && right <= rightQuery)
			return seg[node];

		// ONLY a part of this segment belongs to the query
		Node leftSegment = query(left, mid, L, leftQuery, rightQuery);
		Node rightSegment = query(mid + 1, right, R, leftQuery, rightQuery);
		return merge(leftSegment, rightSegment);
	}

public:
	SegmentTree(const string &arr)
	{
		size = 1;
		int n = arr.size();
		while (size < n)
			size <<= 1;
		seg = vector<Node>(2 * size, 0);
		build(0, size - 1, 0, arr);
	}
	void update(int idx, const ll &val)
	{
		update(0, size - 1, 0, idx, val);
	}
	Node query(int left, int right)
	{
		return query(0, size - 1, 0, left, right);
	}

#undef L
#undef R
#undef mid
};

bool canBePalindrome(SegmentTree &segTree, int st, int end)
{
	if (segTree.query(st, end).forwardHash == segTree.query(st, end).backwardHash) // Already a palindrome
		return true;

	if (segTree.query(st + 1, end - 1).forwardHash == segTree.query(st + 1, end - 1).backwardHash) // If mis-match is on first character
		return true;

	int len = end - st + 1;
	int i = st, j = i + len / 2 - 1, k = end, l = end - len / 2 + 1;
	int L = 0, R = min(j - i, k - l);
	int idx = -1;
	while (L <= R)
	{
		int mid = ((L + R) >> 1);
		if (segTree.query(i + L, i + mid).forwardHash == segTree.query(k - mid, k - L).backwardHash)
			L = mid + 1;
		else
			idx = mid, R = mid - 1;
	}

	// (impossible case because we only check if it's not a palindrome, so there must be a mis-match)
	if (idx == -1) // If there is NO any mis-match
		return true;
	return (segTree.query(i + idx + 1, j).forwardHash == segTree.query(l, k - idx - 1).backwardHash);
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif
	int t = 1;
	ll N, Q;
	cin >> t;
	while (t--)
	{
		cin >> N;
		string str;
		cin >> str;
		SegmentTree segTree(str);
		cin >> Q;
		while (Q--)
		{
			ll query, i, L, R;
			char c;
			cin >> query;
			if (query == 1)
			{
				cin >> i >> c;
				i--;
				segTree.update(i, c);
			}
			else
			{
				cin >> L >> R;
				L--, R--;
				if (canBePalindrome(segTree, L, R))
					cout << "YES\n";
				else
					cout << "NO\n";
			}
		}
	}
	return 0;
}