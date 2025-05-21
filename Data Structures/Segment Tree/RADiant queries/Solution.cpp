#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

vector<ll> Prime, LPF;
bitset<200001> isPrime;

void Linear_Sieve_Of_Eratosthenes(int N)
{
	isPrime.set(); // Initially Assuming all numbers to be primes
	LPF.resize(N + 1);
	isPrime[0] = isPrime[1] = 0; // 0 and 1 are NOT primes
	for (int i{2}; i <= N; i++)
	{
		if (isPrime[i])
		{
			Prime.push_back(i);
			LPF[i] = i; // The least prime factor of a prime number is itself
		}
		for (int j{}; j < (int)Prime.size() and i * Prime[j] <= N and Prime[j] <= LPF[i]; j++)
		{
			isPrime[i * Prime[j]] = 0; // Crossing out all the multiples of prime numbers
			LPF[i * Prime[j]] = Prime[j];
		}
	}
}
vector<vector<ll>> primeFactors;
void primeFactorize()
{
	primeFactors.resize(200001);
	for (ll i = 1; i <= 200000; i++)
	{
		ll N = i;
		while (N > 1)
		{
			ll p = LPF[N];
			primeFactors[i].push_back(p);
			while (N % p == 0)
				N /= p;
		}
	}
}

static int autoCall = (Linear_Sieve_Of_Eratosthenes(200000), primeFactorize(), 0);
const ll mod = 1e9 + 7;
ll mult64(const ll &a, const ll &b)
{
	return __int128_t(a) * b % mod;
}

struct SegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		ll value;
		// Constructors
		Node() {}
		Node(const ll &val) : value(val) {}
	};
	int size;
	vector<Node> seg;
	Node merge(const Node &leftNode, const Node &rightNode)
	{
		Node result;
		result.value = mult64(leftNode.value, rightNode.value);
		return result;
	}
	void build(int left, int right, int node, const vector<ll> &arr)
	{
		if (left == right) // Leaf Node (single element)
		{
			if (left < arr.size()) // Making sure we are inside the boundaries of the array
				seg[node] = arr[left];
			return;
		}
		// Building left node
		build(left, mid, L, arr);

		// Building right node
		build(mid + 1, right, R, arr);

		// Returning to parent nodes
		seg[node] = merge(seg[L], seg[R]);
	}
	void update(int left, int right, int node, int idx, const ll &val, int type)
	{
		if (left == right)
		{
			if (type == 1) // Multiply
				seg[node].value *= val;
			else // Divide
				seg[node].value /= val;
			return;
		}
		if (idx <= mid)
			update(left, mid, L, idx, val, type);
		else
			update(mid + 1, right, R, idx, val, type);
		// Updating while returning to parent nodes
		seg[node] = merge(seg[L], seg[R]);
	}
	Node query(int left, int right, int node, int leftQuery, int rightQuery)
	{
		// Out of range
		if (right < leftQuery || left > rightQuery)
			return 1; // A value that doesn't affect the query

		// The whole range is the answer
		if (left >= leftQuery && right <= rightQuery)
			return seg[node];

		// ONLY a part of this segment belongs to the query
		Node leftSegment = query(left, mid, L, leftQuery, rightQuery);
		Node rightSegment = query(mid + 1, right, R, leftQuery, rightQuery);
		return merge(leftSegment, rightSegment);
	}

public:
	SegmentTree(const vector<ll> &arr)
	{
		size = 1;
		int n = arr.size();
		while (size < n)
			size <<= 1;
		seg = vector<Node>(2 * size, 1);
		build(0, size - 1, 0, arr);
	}
	void update(int idx, const ll &val, int type)
	{
		update(0, size - 1, 0, idx, val, type);
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

bool pred(const tuple<int, int, int> &T1, const tuple<int, int, int> &T2)
{
	return get<1>(T1) > get<1>(T2);
}

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
		cin >> N >> Q;
		vector<ll> vc(N);
		for (int i{}; i < N; i++)
			cin >> vc[i];
		vector<stack<int>> lastPos(200001);
		for (int i{}; i < N; i++)
		{
			for (const ll &p : primeFactors[vc[i]])
				lastPos[p].push(i);
		}
		SegmentTree segTree(vector<ll>(N, 1));
		for (const ll &p : Prime)
		{
			if (!lastPos[p].empty())
				segTree.update(lastPos[p].top(), p, 1);
		}

		vector<tuple<int, int, int>> queries(Q);
		int ID = 0;
		for (auto &[L, R, id] : queries)
		{
			cin >> L >> R;
			L--, R--; // To be 0-based
			id = ID++;
		}
		sort(queries.begin(), queries.end(), pred);
		vector<int> res(Q);
		int currRight = N - 1;
		for (const auto &[L, R, id] : queries)
		{
			// Adjust state for new right boundary
			while (currRight > R)
			{
				for (const ll &p : primeFactors[vc[currRight]])
				{
					// Remove this prime factor from the current position
					if (!lastPos[p].empty() && lastPos[p].top() >= currRight)
					{
						segTree.update(currRight, p, 2); // Divide
						lastPos[p].pop();
					}
					// Find the previous occurrence
					if (!lastPos[p].empty())
						segTree.update(lastPos[p].top(), p, 1); // Multiply
				}
				currRight--;
			}

			// Store the result for this query
			res[id] = segTree.query(L, R);
		}
		for (int id{}; id < Q; id++)
			cout << res[id] << endl;
	}
	return 0;
}