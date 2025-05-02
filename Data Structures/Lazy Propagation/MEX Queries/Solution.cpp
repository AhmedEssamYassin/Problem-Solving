#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

class CompressedRange
{
private:
	vector<ll> init;
	void compress(vector<ll> &vec)
	{
		sort(vec.begin(), vec.end());
		vec.erase(unique(vec.begin(), vec.end()), vec.end());
	}

public:
	CompressedRange(vector<ll> &vec)
	{
		init = vec;
		compress(init);
	}
	int index(ll val)
	{
		return lower_bound(init.begin(), init.end(), val) - init.begin();
	}
	ll initVal(int idx)
	{
		return init[idx];
	}
};

struct LazySegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		ll value = 0;
		Node() {}
		Node(const ll &N) : value(N) {}
	};
	struct LazyNode
	{
		ll assignValue = -1;
		bool flip = false;
		LazyNode() {}
		LazyNode(const ll &N, const bool &f) : assignValue(N), flip(f) {}
		LazyNode operator^=(const LazyNode &RHS)
		{
			flip ^= RHS.flip;
			return *this;
		}
	};
	int size;
	vector<Node> seg;
	vector<LazyNode> lazy;
	Node merge(const Node &leftNode, const Node &rightNode)
	{
		Node res = (leftNode.value + rightNode.value);
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
		if (lazy[node].assignValue != -1) // Assign overwrites anything
		{
			seg[node].value = (right - left + 1) * lazy[node].assignValue;
			// If the node is not a leaf
			if (left != right)
			{
				// Update the lazy values for the left child
				lazy[L] = {lazy[node].assignValue, false};
				// Update the lazy values for the right child
				lazy[R] = {lazy[node].assignValue, false};
			}
			// Reset the lazy value
			lazy[node] = {-1, false};
		}
		if (lazy[node].flip == true)
		{
			seg[node].value = (right - left + 1) - seg[node].value;
			// If the node is not a leaf
			if (left != right)
			{
				// Update the lazy values for the left child
				if (lazy[L].assignValue == -1)
					lazy[L] ^= lazy[node];
				else
					lazy[L].assignValue ^= 1; // As if we are assigning then flipping

				// Update the lazy values for the right child
				if (lazy[R].assignValue == -1)
					lazy[R] ^= lazy[node];
				else
					lazy[R].assignValue ^= 1; // As if we are assigning then flipping
			}
			// Reset the lazy value
			lazy[node].flip = false;
		}
	}
	void update(int left, int right, int node, int leftQuery, int rightQuery, ll type, const ll &val)
	{
		push(left, right, node);
		// If the range is invalid, return
		if (left > rightQuery || right < leftQuery)
			return;
		// If the range matches the segment
		if (left >= leftQuery && right <= rightQuery)
		{
			// Update the lazy value
			if (type == 1)				   // Assign
				lazy[node] = {val, false}; // Assignment overwrites Flipping

			else if (type == 2) // Flip
				lazy[node].flip = true;

			// Apply the update immediately
			push(left, right, node);
			return;
		}
		// Recursively update the left child
		update(left, mid, L, leftQuery, rightQuery, type, val);
		// Recursively update the right child
		update(mid + 1, right, R, leftQuery, rightQuery, type, val);
		// Merge the children values
		seg[node] = merge(seg[L], seg[R]);
	}
	ll query(int left, int right, int node)
	{
		// Apply the pending updates if any
		push(left, right, node);
		if (left == right)
			return left;

		// Ensure lazy updates on left segment are applied before checking
		push(left, mid, L);
		if (seg[L].value == (mid - left + 1)) // If left side has all 1s, search right
			return query(mid + 1, right, R);

		else // Otherwise, search left
			return query(left, mid, L);
	}

public:
	LazySegmentTree(const vector<ll> &arr)
	{
		size = 1;
		int n = arr.size();
		while (size < n)
			size <<= 1;
		seg = vector<Node>(2 * size, 0);
		lazy = vector<LazyNode>(2 * size, {-1, false});
		build(0, size - 1, 0, arr);
	}
	void update(int left, int right, ll type, const ll &val)
	{
		update(0, size - 1, 0, left, right, type, val);
	}
	ll query()
	{
		return query(0, size - 1, 0);
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
	ll N, K;
	// cin >> t;
	while (t--)
	{
		cin >> N;
		vector<tuple<ll, ll, ll>> queries(N);
		vector<ll> vc{1};
		for (auto &[type, L, R] : queries)
		{
			cin >> type >> L >> R;
			vc.push_back(L);
			vc.push_back(R);
			vc.push_back(R + 1); // To handle the case where all range is existing so MEX =  the next number
		}
		CompressedRange comp(vc);
		for (auto &[type, L, R] : queries)
		{
			L = comp.index(L);
			R = comp.index(R);
		}
		LazySegmentTree segTree(vector<ll>(vc.size(), 0));
		for (const auto &[type, L, R] : queries)
		{
			if (type == 1)
				segTree.update(L, R, 1, 1);
			else if (type == 2)
				segTree.update(L, R, 1, 0);
			else // 3
				segTree.update(L, R, 2, 1);
			cout << comp.initVal(segTree.query()) << endl;
		}
	}
	return 0;
}