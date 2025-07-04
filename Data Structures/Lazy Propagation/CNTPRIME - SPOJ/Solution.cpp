#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

bitset<1000001> isPrime;

void Sieve_Of_Eratosthenes(int N)
{
	isPrime.set();
	isPrime[0] = 0;
	isPrime[1] = 0;
	for (int i{2}; i * i <= N; i++)
	{
		if (isPrime[i])
		{
			for (int j = 2 * i; j <= N; j += i)
				isPrime[j] = 0; // Crossing out all the multiples of prime numbers
		}
	}
}

struct LazySegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		ll CNTPRIME{};
		Node() {}
		Node(const ll &N) : CNTPRIME(isPrime[N]) {}
	};
	struct LazyNode
	{
		ll value{};
		LazyNode() {}
		LazyNode(const ll &N) : value(N) {}
	};
	int size;
	vector<Node> seg;
	vector<LazyNode> lazy;
	Node merge(const Node &leftNode, const Node &rightNode)
	{
		Node res;
		res.CNTPRIME = leftNode.CNTPRIME + rightNode.CNTPRIME;
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
			seg[node].CNTPRIME = (right - left + 1) * isPrime[lazy[node].value];
			// If the node is not a leaf
			if (left != right)
			{
				// Update the lazy values for the left child
				lazy[L] = lazy[node];
				// Update the lazy values for the right child
				lazy[R] = lazy[node];
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
			return 0;

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

		seg = vector<Node>(2 * size, 0);
		lazy = vector<LazyNode>(2 * size, 0);
		build(0, size - 1, 0, arr);
	}
	void update(int left, int right, const ll &val)
	{
		update(0, size - 1, 0, left, right, val);
	}
	ll query(int leftQuery, int rightQuery)
	{
		Node ans = query(0, size - 1, 0, leftQuery, rightQuery);
		return ans.CNTPRIME;
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
	cin >> t;
	Sieve_Of_Eratosthenes(1000000);
	for (int T = 1; T <= t; T++)
	{
		cout << "Case " << T << ":" << endl;
		cin >> N >> Q;
		vector<ll> vc(N);
		for (int i{}; i < N; i++)
			cin >> vc[i];
		LazySegmentTree segTree(vc);
		while (Q--)
		{
			ll query, L, R, x, V;
			cin >> query;
			if (query == 0)
			{
				cin >> L >> R >> V;
				segTree.update(L - 1, R - 1, V);
			}
			else
			{
				cin >> L >> R;
				cout << segTree.query(L - 1, R - 1) << endl;
			}
		}
	}
	return 0;
}