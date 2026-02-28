#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

// Flattening binary trees is useful to do subtree queries.

vector<int> primes, LPF;
bitset<1001> isPrime;
bitset<1000> pMask;
void linearSieveOfEratosthenes(int N)
{
	isPrime.set(); // Initially Assuming all numbers to be primes
	LPF.resize(N + 1);
	isPrime[0] = isPrime[1] = 0; // 0 and 1 are NOT primes
	for (long long i{2}; i <= N; i++)
	{
		if (isPrime[i])
		{
			primes.push_back(i);
			LPF[i] = i; // The least prime factor of a prime number is itself
		}
		for (long long j{}; j < (int)primes.size() && i * primes[j] <= N && primes[j] <= LPF[i]; j++)
		{
			isPrime[i * primes[j]] = 0; // Crossing out all the multiples of prime numbers
			LPF[i * primes[j]] = primes[j];
		}
	}
	for (const int &p : primes)
		pMask[p] = 1;
}
static int autoCall = (linearSieveOfEratosthenes(1000), 0);

int m;
bitset<1000> validMask;
struct LazySegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		bitset<1000> remMask;
		Node() {}
		Node(const ll &N) { remMask[N % m] = 1; }
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
		res.remMask = (leftNode.remMask | rightNode.remMask);
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
		// Propagate the value
		if (lazy[node].value == 0)
			return;
		int x = lazy[node].value % m;
		seg[node].remMask = (seg[node].remMask << x) | (seg[node].remMask >> (m - x));
		seg[node].remMask &= validMask;

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
			lazy[node].value += val;
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
		return (ans.remMask & pMask).count();
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
		ll N;
		cin >> N >> m;
		validMask.reset();
		for (int i{}; i < m; i++)
			validMask[i] = 1;
		vector<ll> a(N + 1);
		for (int i{1}; i <= N; i++)
			cin >> a[i];
		vector<vector<ll>> Tree(N + 1);
		for (int i{}; i < N - 1; i++)
		{
			ll u, v;
			cin >> u >> v;
			Tree[u].push_back(v);
			Tree[v].push_back(u);
		}
		ll root = 1;
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
		eulerTour(root, -1);
		vector<ll> nodes(N);
		for (int u = 1; u <= N; u++)
			nodes[in[u]] = a[u];

		LazySegmentTree segTree(nodes);
		ll Q;
		cin >> Q;
		while (Q--)
		{
			ll type, v, x;
			cin >> type;
			if (type == 1)
			{
				cin >> v >> x;
				segTree.update(in[v], out[v], x);
			}
			else
			{
				cin >> v;
				cout << segTree.query(in[v], out[v]) << endl;
			}
		}
	}
	return 0;
}