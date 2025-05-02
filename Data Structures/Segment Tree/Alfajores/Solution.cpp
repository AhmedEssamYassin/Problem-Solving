#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct SegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		ll minVal;
		// Constructors
		Node() {}
		Node(ll val) : minVal(val) {}
	};
	int size;
	vector<Node> seg;
	Node merge(Node &leftNode, Node &rightNode)
	{
		Node res;
		res.minVal = min(leftNode.minVal, rightNode.minVal);
		return res;
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
	void update(int left, int right, int node, int idx, const ll &val)
	{
		if (left == right)
		{
			seg[node] = val;
			return;
		}
		if (idx <= mid)
			update(left, mid, L, idx, val);
		else
			update(mid + 1, right, R, idx, val);
		// Updating while returning to parent nodes
		seg[node] = merge(seg[L], seg[R]);
	}
	ll query(int left, int right, int node, int leftQuery, int rightQuery, ll rem)
	{
		if (seg[node].minVal > rem || left > rightQuery || right < leftQuery) // Invalid subtree
			return -1;
		if (left == right)
			return left;

		ll traversingLeft = query(left, mid, L, leftQuery, rightQuery, rem);
		if (traversingLeft == -1) // If the left segment doesn't contain any value <= rem, go right
			return query(mid + 1, right, R, leftQuery, rightQuery, rem);
		else
			return traversingLeft;
	}

public:
	SegmentTree(const vector<ll> &arr)
	{
		size = 1;
		int n = arr.size();
		while (size < n)
			size <<= 1;
		seg = vector<Node>(2 * size, LLONG_MAX);
		build(0, size - 1, 0, arr);
	}
	void update(int idx, const ll &val)
	{
		update(0, size - 1, 0, idx, val);
	}
	ll query(int left, int right, ll rem)
	{
		return query(0, size - 1, 0, left, right, rem);
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
	ll N, M;
	// cin >> t;
	while (t--)
	{
		cin >> N >> M;
		vector<ll> A(N), E(M);
		for (int i{}; i < N; i++)
			cin >> A[i];
		for (int i{}; i < M; i++)
			cin >> E[i];
		SegmentTree segTree(E);
		for (int i{}; i < N; i++)
		{
			ll rem = A[i];
			for (int j = segTree.query(0, M - 1, rem); j >= 0 && j < M && rem > 0; j = segTree.query(0, M - 1, rem))
				rem %= E[j];
			cout << rem << " ";
		}
	}
	return 0;
}