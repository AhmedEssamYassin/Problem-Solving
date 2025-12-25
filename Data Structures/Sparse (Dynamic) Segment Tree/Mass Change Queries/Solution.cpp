#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct DynamicSegmentTree
{
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		ll value;
		Node *L, *R; // Pointers to left child and right child

		Node() // Constructor
		{
			value = 0;
			L = nullptr;
			R = nullptr;
		}
		Node(const ll &val)
		{
			value = val;
			L = nullptr;
			R = nullptr;
		}

		void createChildren() // Construct Childs
		{
			if (L == nullptr)
				L = new Node();
			if (R == nullptr)
				R = new Node();
		}
		// Will cause RTE, when we delete a node, it will unintentionally delete its children
		// ~Node() // Destructor. Notice the "~" character before the struct name.
		// {
		// 	if (L != nullptr)
		// 		delete L;
		// 	if (R != nullptr)
		// 		delete R;
		// 	L = nullptr;
		// 	R = nullptr;
		// }
	};
	ll N;
	void insert(Node *&curr, int left, int right, ll idx)
	{
		if (curr == nullptr)
			curr = new Node();
		if (left == right)
			return;
		if (idx <= mid)
			insert(curr->L, left, mid, idx);
		else
			insert(curr->R, mid + 1, right, idx);
	}

public:
	Node *root;
	DynamicSegmentTree(ll rangeSize = 1e9)
	{
		root = new Node();
		N = rangeSize;
	}
	void insert(ll idx)
	{
		insert(root, 0, N - 1, idx);
	}
	static void transfer(Node *&xRoot, Node *&yRoot, int left, int right, int leftQuery, int rightQuery)
	{
		// No overlap with query range
		if (left > rightQuery || right < leftQuery)
			return;

		// Complete overlap with range, i.e., Complete range transfer
		if (left >= leftQuery && right <= rightQuery)
		{
			if (xRoot == nullptr)
				return;

			if (yRoot == nullptr)
			{
				swap(xRoot, yRoot);
				return;
			}

			transfer(xRoot->L, yRoot->L, left, mid, leftQuery, rightQuery);
			transfer(xRoot->R, yRoot->R, mid + 1, right, leftQuery, rightQuery);
			delete xRoot;
			xRoot = nullptr;
		}
		else // Partial overlapping
		{
			// Create local variables that can be passed by reference
			Node *xLeft = xRoot ? xRoot->L : nullptr;
			Node *xRight = xRoot ? xRoot->R : nullptr;
			Node *yLeft = yRoot ? yRoot->L : nullptr;
			Node *yRight = yRoot ? yRoot->R : nullptr;

			// Now safely recurse
			transfer(xLeft, yLeft, left, mid, leftQuery, rightQuery);
			transfer(xRight, yRight, mid + 1, right, leftQuery, rightQuery);

			// Update the pointers in the original nodes
			if (xRoot)
			{
				if (xLeft == nullptr && xRight == nullptr)
				{
					delete xRoot;
					xRoot = nullptr;
				}
				else
				{
					xRoot->L = xLeft;
					xRoot->R = xRight;
				}
			}
			if ((yLeft || yRight) && (yRoot == nullptr))
				yRoot = new Node();
			if (yRoot)
			{
				yRoot->L = yLeft;
				yRoot->R = yRight;
			}
		}
	}
	static void recover(struct Node *node, vector<ll> &arr, int left, int right, ll val)
	{
		if (node == nullptr)
			return;
		if (left == right)
		{
			arr[left] = val;
			return;
		}
		recover(node->L, arr, left, mid, val);
		recover(node->R, arr, mid + 1, right, val);
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
	ll N, Q;
	// cin >> t;
	while (t--)
	{
		cin >> N;
		map<ll, DynamicSegmentTree> mp;
		for (int i{}; i < N; i++)
		{
			ll x;
			cin >> x;
			if (mp.count(x) == 0)
				mp[x] = DynamicSegmentTree(N);
			mp[x].insert(i);
		}
		cin >> Q;
		for (int i{}; i < Q; i++)
		{
			ll L, R, x, y;
			cin >> L >> R >> x >> y;
			L--, R--;
			if (x != y && mp.count(x))
			{
				// Create y entry if it doesn't exist
				if (mp.count(y) == 0)
					mp[y] = DynamicSegmentTree(N);
				DynamicSegmentTree::transfer(mp[x].root, mp[y].root, 0, N - 1, L, R);
			}
		}
		vector<ll> arr(N);
		for (auto &[val, segTree] : mp)
			DynamicSegmentTree::recover(segTree.root, arr, 0, N - 1, val);
		for (int i{}; i < N; i++)
			cout << arr[i] << " ";
	}
	return 0;
}