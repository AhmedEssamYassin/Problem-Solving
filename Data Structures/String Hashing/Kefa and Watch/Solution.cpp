#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

function<uint64_t()> random_address = []() -> uint64_t
{
	char *p = new char;
	delete p;
	return uint64_t(p);
};
const uint64_t SEED = chrono::steady_clock::now().time_since_epoch().count() * (random_address() | 1);
std::mt19937 rnd(SEED);
#define rng(l, r) uniform_int_distribution<int64_t>(l, r)(rnd)

/*
Large Primes for hash
1000000007
For all coming primes use __int128_t in mult64()
10000000019
100000000003
1000000000039
10000000000037
100000000000031
1000000000000037
10000000000000061
2305843009213693951 = (1LL << 61) - 1

Larger primes take more time
1e9 + 7 is usually sufficient for most of the hashing problems
*/

const ll mod = (1LL << 61) - 1;

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

ll modPow(ll N, ll power, ll mod)
{
	ll res{1};
	N %= mod;
	while (power)
	{
		if (power & 1)
			res = mult64(res, N);
		N = mult64(N, N);
		power >>= 1;
	}
	return res;
}
ll b1 = rng(100, 1000000000), b2 = rng(b1 + 10, 1000000000);
ll b1I = modPow(b1, mod - 2, mod), b2I = modPow(b2, mod - 2, mod);
vector<ll> Pb1, Pb2, sumB1, sumB2;
void precompute(ll maxSize)
{
	Pb1 = Pb2 = sumB1 = sumB2 = vector<ll>(maxSize + 1, 1);
	for (int i = 1; i <= maxSize; i++)
	{
		Pb1[i] = mult64(Pb1[i - 1], b1);
		Pb2[i] = mult64(Pb2[i - 1], b2);
		sumB1[i] = add64(sumB1[i - 1], Pb1[i]);
		sumB2[i] = add64(sumB2[i - 1], Pb2[i]);
	}
}

static int autoCall = (precompute(1e5), 0);

class Hash
{
	using pll = pair<ll, ll>;
	ll size{};

public:
	pll code{};

	explicit Hash(pair<ll, ll> x = {}, ll sz = {}) : code(std::move(x)), size(sz) {}

	Hash(const ll &x) : code({x % mod, x % mod}), size(1) {}

	Hash(const string &x) : code(), size(0)
	{
		for (const char &c : x)
			*this = *(this) + c;
	}

	void pop_front(int x)
	{
		code.first = sub64(code.first, mult64(Pb1[--size], x));
		code.second = sub64(code.second, mult64(Pb2[size], x));
	}

	void pop_back(int x)
	{
		code.first = mult64((code.first - x + mod), b1I);
		code.second = mult64((code.second - x + mod), b2I);
		size--;
	}
	void clear()
	{
		code = {}, size = 0;
	}
	Hash operator+(const Hash &o)
	{
		Hash ans;
		ans.code = {add64(mult64(code.first, Pb1[o.size]), o.code.first),
					add64(mult64(code.second, Pb2[o.size]), o.code.second)};
		ans.size = size + o.size;
		return ans;
	}
	friend Hash operator+(const Hash &f, const Hash &o)
	{
		return Hash({add64(mult64(f.code.first, Pb1[o.size]), o.code.first),
					 add64(mult64(f.code.second, Pb2[o.size]), o.code.second)},
					f.size + o.size);
	}
	bool operator<(const Hash &o) const
	{
		if (code == o.code)
			return size < o.size;
		return code < o.code;
	}
	bool operator==(const Hash &o) const
	{
		return size == o.size && code == o.code;
	}
	bool operator!=(const Hash &o) const
	{
		return size != o.size || code != o.code;
	}
};

// Rabin-Karp Algorithm
struct HashRange
{
	vector<Hash> p, s;
	HashRange(const string &t) : p(t.size()), s(t.size())
	{
		if (t.empty())
			return;
		p.front() = t.front();
		for (int i = 1; i < t.size(); i++)
			p[i] = p[i - 1] + t[i];
		s.back() = t.back();
		for (int i = int(t.size()) - 2; i >= 0; i--)
			s[i] = s[i + 1] + t[i];
	}
	Hash get(int l, int r) const // 0-based indices
	{
		if (l > r)
			return Hash();
		if (!l)
			return p[r];
		return Hash({sub64(p[r].code.first, mult64(p[l - 1].code.first, Pb1[r - l + 1])),
					 sub64(p[r].code.second, mult64(p[l - 1].code.second, Pb2[r - l + 1]))},
					r - l + 1);
	}
	Hash inv(int l, int r) const // 0-based indices
	{
		if (l > r)
			return Hash();
		if (r + 1 == s.size())
			return s[l];
		return Hash({sub64(s[l].code.first, mult64(s[r + 1].code.first, Pb1[r - l + 1])),
					 sub64(s[l].code.second, mult64(s[r + 1].code.second, Pb2[r - l + 1]))},
					r - l + 1);
	}
	void concatenate(const string &t)
	{
		if (t.empty())
			return;
		bool chk = false;
		if (chk = p.empty())
			p.push_back(t[0]);
		for (int i = 0 + chk; i < t.size(); i++)
			p.push_back(p.back() + t[i]);
	}
	void pop_back()
	{
		if (!p.empty())
			p.pop_back();
	}
};
////////////////////////////////////////////////////////////////////////////////////

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
		}
		else
		{
			// Recursively build the left child
			build(left, mid, L, str);
			// Recursively build the right child
			build(mid + 1, right, R, str);
			// Merge the children values
			seg[node] = merge(seg[L], seg[R]);
		}
	}
	void push(int left, int right, int node)
	{
		// Propagate the value
		if (lazy[node].lazyValue != -1)
		{
			seg[node].h.code = {mult64(sumB1[right - left], lazy[node].lazyValue), mult64(sumB2[right - left], lazy[node].lazyValue)};
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
		}
		else
		{
			// Recursively update the left child
			update(left, mid, L, leftQuery, rightQuery, val);
			// Recursively update the right child
			update(mid + 1, right, R, leftQuery, rightQuery, val);
			// Merge the children values
			seg[node] = merge(seg[L], seg[R]);
		}
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
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
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