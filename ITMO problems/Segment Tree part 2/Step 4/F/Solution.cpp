#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"
#define INF LLONG_MAX

struct LazyDynamicSegmentTree
{
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		ll sum;
		ll maxPrefSum;
		Node *L, *R; // Pointers to left child and right child

		Node() // Constructor
		{
			sum = 0;
			maxPrefSum = 0;
			L = nullptr;
			R = nullptr;
		}
		Node(const ll &val, const ll &maxVal)
		{
			sum = val;
			maxPrefSum = maxVal;
			L = nullptr;
			R = nullptr;
		}
		void createChildren(const ll &val = 0, const ll &maxVal = 0) // Construct Childs
		{
			if (L == nullptr)
				L = new Node(val, maxVal);
			if (R == nullptr)
				R = new Node(val, maxVal);
		}
		~Node() // Destructor. Notice the "~" character before the struct name.
		{
			delete L;
			delete R;
			L = nullptr;
			R = nullptr;
		}
	};
	struct LazyNode
	{
		ll value;
		LazyNode *L, *R; // Pointers to left child and right child

		LazyNode() // Constructor
		{
			value = INF;
			L = nullptr;
			R = nullptr;
		}
		LazyNode(const ll &val)
		{
			value = val;
			L = nullptr;
			R = nullptr;
		}
		void createChildren(const ll &val = INF) // Construct Childs
		{
			if (L == nullptr)
				L = new LazyNode(val);
			if (R == nullptr)
				R = new LazyNode(val);
		}
		~LazyNode() // Destructor. Notice the "~" character before the struct name.
		{
			delete L;
			delete R;
			L = nullptr;
			R = nullptr;
		}
	};
	ll N;
	Node *segRoot;
	LazyNode *lazyRoot;

	void merge(Node *&segNode)
	{
		segNode->sum = (segNode->L->sum + segNode->R->sum);
		segNode->maxPrefSum = max(segNode->L->maxPrefSum, segNode->L->sum + segNode->R->maxPrefSum);
	}

	void push(ll left, ll right, Node *&segNode, LazyNode *lazyNode)
	{
		// Propagate the value
		if (segNode == nullptr || lazyNode == nullptr || lazyNode->value == INF)
			return;
		// (a + b + c + d + e) --> (x + x + x + x + x) --> (right - left + 1) * x
		segNode->sum = (right - left + 1) * lazyNode->value;
		segNode->maxPrefSum = max(0LL, segNode->sum);
		// If the node is not a leaf
		if (left != right)
		{
			lazyNode->createChildren();
			// Update the lazy values for the left child
			lazyNode->L->value = lazyNode->value;

			// Update the lazy values for the right child
			lazyNode->R->value = lazyNode->value;
		}
		// Reset the lazy value
		lazyNode->value = INF;
	}
	void update(ll left, ll right, Node *&segNode, LazyNode *&lazyNode, ll leftQuery, ll rightQuery, const ll &val)
	{
		push(left, right, segNode, lazyNode);
		// If the range is invalid, return
		if (left > rightQuery || right < leftQuery)
			return;
		// If the range matches the segment
		if (left >= leftQuery && right <= rightQuery)
		{
			// Update the lazy value
			lazyNode->value = val;
			// Apply the update immediately
			push(left, right, segNode, lazyNode);
			return;
		}
		segNode->createChildren();
		lazyNode->createChildren(INF);
		// Recursively update the left child
		update(left, mid, segNode->L, lazyNode->L, leftQuery, rightQuery, val);
		// Recursively update the right child
		update(mid + 1, right, segNode->R, lazyNode->R, leftQuery, rightQuery, val);
		// Merge the children values
		merge(segNode);
	}
	ll query(ll left, ll right, Node *&segNode, LazyNode *lazyNode, ll h)
	{
		// Apply the pending updates if any
		push(left, right, segNode, lazyNode);
		if (left == right)
			return left - (segNode->maxPrefSum > h);
		segNode->createChildren();
		lazyNode->createChildren(INF);
		push(left, mid, segNode->L, lazyNode->L);
		if (segNode->L->maxPrefSum > h)
			return query(left, mid, segNode->L, lazyNode->L, h);
		else
			return query(mid + 1, right, segNode->R, lazyNode->R, h - segNode->L->sum);
	}

public:
	LazyDynamicSegmentTree(ll rangeSize = 2e9)
	{
		segRoot = new Node();
		lazyRoot = new LazyNode(INF);
		N = rangeSize;
	}
	void update(ll left, ll right, const ll &val)
	{
		update(1, N, segRoot, lazyRoot, left, right, val);
	}
	ll query(ll h)
	{
		ll ans = query(1, N, segRoot, lazyRoot, h);
		return ans;
	}
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
	ll N;
	// cin >> t;
	while (t--)
	{
		cin >> N;
		char C;
		ll a, b, D, h;
		LazyDynamicSegmentTree segTree(N);
		segTree.update(1, N, 0); // Initially, make all elements 0
		while (cin >> C)
		{
			if (C == 'E')
				return 0;
			if (C == 'I')
			{
				cin >> a >> b >> D;
				segTree.update(a, b, D);
			}
			else if (C == 'Q')
			{
				cin >> h;
				// The summation is NOT monotonic!! We cannot binary search
				cout << segTree.query(h) << endl;
			}
		}
	}
	return 0;
}