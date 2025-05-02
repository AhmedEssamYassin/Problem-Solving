#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct MergeSortSegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		vector<ll> range;
		// Constructors
		Node() {}
		Node(ll val)
		{
			range.push_back(val);
		}
	};
	int size;
	vector<Node> seg;
	vector<ll> allValues;
	Node merge(const Node &leftNode, const Node &rightNode)
	{
		Node res;
		int i = 0; // Index for leftNode
		int j = 0; // Index for rightNode

		// Merge the two sorted ranges
		while (i < leftNode.range.size() && j < rightNode.range.size())
		{
			if (leftNode.range[i] <= rightNode.range[j])
				res.range.push_back(leftNode.range[i++]);
			else
				res.range.push_back(rightNode.range[j++]);
		}

		// Add any remaining elements from leftNode
		while (i < leftNode.range.size())
			res.range.push_back(leftNode.range[i++]);

		// Add any remaining elements from rightNode
		while (j < rightNode.range.size())
			res.range.push_back(rightNode.range[j++]);

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
	int countLessEqual(int left, int right, int node, int leftQuery, int rightQuery, ll x)
	{
		if (right < leftQuery || left > rightQuery)
			return 0;
		if (left >= leftQuery && right <= rightQuery)
			return upper_bound(seg[node].range.begin(), seg[node].range.end(), x) - seg[node].range.begin();
		return countLessEqual(left, mid, L, leftQuery, rightQuery, x) +
			   countLessEqual(mid + 1, right, R, leftQuery, rightQuery, x);
	}

public:
	MergeSortSegmentTree(const vector<ll> &arr)
	{
		size = 1;
		int n = arr.size();
		while (size < n)
			size <<= 1;
		allValues = arr;
		sort(allValues.begin(), allValues.end());
		allValues.erase(unique(allValues.begin(), allValues.end()), allValues.end());
		seg = vector<Node>(2 * size, Node());
		build(0, size - 1, 0, arr);
	}
	void update(int idx, const ll &val)
	{
		update(0, size - 1, 0, idx, val);
	}
	int countGreaterThan(int leftQuery, int rightQuery, ll x)
	{
		int countLessEq = countLessEqual(0, size - 1, 0, leftQuery, rightQuery, x);
		return (rightQuery - leftQuery + 1) - countLessEq;
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
	// cin >> t;
	while (t--)
	{
		cin >> N;
		vector<ll> vc(N);
		for (int i{}; i < N; i++)
			cin >> vc[i];
		MergeSortSegmentTree segTree(vc);
		cin >> Q;
		for (int i{}; i < Q; i++)
		{
			ll L, R, k;
			cin >> L >> R >> k;
			L--, R--;
			cout << segTree.countGreaterThan(L, R, k) << endl;
		}
	}
	return 0;
}
