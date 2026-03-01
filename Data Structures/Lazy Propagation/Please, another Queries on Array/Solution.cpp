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

int pos[301];
vector<int> primes;
bitset<3001> isPrime;
void linearSieveOfEratosthenes(int N)
{
	isPrime.set();				 // Initially Assuming all numbers to be primes
	isPrime[0] = isPrime[1] = 0; // 0 and 1 are NOT primes
	for (long long i{2}; i <= N; i++)
	{
		if (isPrime[i])
			primes.push_back(i);
		for (long long j = 0; j < (int)primes.size() && i * primes[j] <= N; j++)
		{
			isPrime[i * primes[j]] = 0;
			if (i % primes[j] == 0)
				break;
		}
	}
	int i = 0;
	for (const int &p : primes)
		pos[p] = i++;
}
static int autoCall = (linearSieveOfEratosthenes(300), 0);

inline void setIthBit(ll &N, uint16_t i) // 0-based
{
	N |= (1LL << i);
}

void primeFactorize(ll N, ll &primeSet)
{
	if (N <= 1)
		return;
	if (!(N & 1))
		setIthBit(primeSet, pos[2]);

	while (not(N & 1))
		N >>= 1;

	for (ll p = 3; p * p <= N && N > 1; p += 2)
	{
		if (N % p == 0)
			setIthBit(primeSet, pos[p]);
		while (N % p == 0)
			N /= p;
	}
	if (N > 1)
		setIthBit(primeSet, pos[N]);
}

struct LazySegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		ll value;
		ll primeSet;
		Node() {}
		Node(const ll &N)
		{
			value = N;
			primeSet = 0;
			primeFactorize(N, primeSet);
		}
		Node operator*(const Node &RHS)
		{
			value = mult64(value, RHS.value);
			primeSet |= RHS.primeSet;
			return *this;
		}
	};
	// LazyNode is identical to the Node of segment tree
	int size;
	vector<Node> seg, lazy;
	Node merge(const Node &leftNode, const Node &rightNode)
	{
		Node res;
		res.value = mult64(leftNode.value, rightNode.value);
		res.primeSet = (leftNode.primeSet | rightNode.primeSet);
		return res;
	}
	void build(int left, int right, int node, const vector<ll> &arr)
	{
		// Initialize the lazy array with ones
		lazy[node] = 1;
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
		if (lazy[node].value == 1)
			return;
		// Propagate the value
		ll val = modPow(lazy[node].value, right - left + 1);
		seg[node].value = mult64(seg[node].value, val);
		seg[node].primeSet = (seg[node].primeSet | lazy[node].primeSet);
		// If the node is not a leaf
		if (left != right)
		{
			// Update the lazy values for the left child
			lazy[L] = (lazy[L] * lazy[node]);
			// Update the lazy values for the right child
			lazy[R] = (lazy[R] * lazy[node]);
		}
		// Reset the lazy value
		lazy[node] = 1;
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
			lazy[node] = (lazy[node] * val);

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
			return 1; // The node constructor will make it {0, 1}

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

		seg = vector<Node>(2 * size, 1);
		lazy = vector<Node>(2 * size, 1);
		build(0, size - 1, 0, arr);
	}
	void update(int left, int right, const ll &val)
	{
		update(0, size - 1, 0, left, right, val);
	}
	pair<ll, ll> query(int left, int right)
	{
		Node ans = query(0, size - 1, 0, left, right);
		return {ans.primeSet, ans.value};
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
	ll N, Q;
	// cin >> t;
	while (t--)
	{
		cin >> N >> Q;
		vector<ll> vc(N);
		for (int i{}; i < N; i++)
			cin >> vc[i];
		ll modInverse[63];
		for (int i{}; i < 63; i++)
			modInverse[i] = modPow(primes[i], mod - 2); // mod inverse
		LazySegmentTree segTree(vc);
		while (Q--)
		{
			string query;
			ll L, R, val;
			cin >> query >> L >> R;
			L--, R--;
			if (query == "MULTIPLY")
			{
				cin >> val;
				segTree.update(L, R, val);
			}
			else
			{
				auto [primeSet, value] = segTree.query(L, R);
				ll res = value;
				for (int i{}; i < 63; i++)
				{
					if ((primeSet & (1LL << i)))
						res = mult64(res, mult64((primes[i] - 1), modInverse[i]));
				}
				cout << res << endl;
			}
		}
	}
	return 0;
}