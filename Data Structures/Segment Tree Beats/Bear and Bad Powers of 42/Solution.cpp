#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"
const ll INF = 4e18;

struct LazySegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	vector<ll> pw;
	struct Node
	{
		ll val, mn;
		Node() { val = 0, mn = INF; }
		Node(const ll &A, const ll &B) : val(A), mn(B) {}
	};
	struct LazyNode
	{
		ll set, add;
		LazyNode() { set = add = 0; }
		LazyNode(const ll &S, const ll &A) : set(S), add(A) {}
		LazyNode &operator+=(const LazyNode &RHS)
		{
			if (RHS.set != 0)
				set = RHS.set, add = RHS.add;
			else
				add += RHS.add;
			return *this;
		}
	};
	int size;
	vector<Node> seg;
	vector<LazyNode> lazy;
	ll dist(const ll &v) { return *lower_bound(pw.begin(), pw.end(), v) - v; }
	Node merge(const Node &leftNode, const Node &rightNode)
	{
		Node res;
		res.mn = min(leftNode.mn, rightNode.mn);
		return res;
	}
	void build(int left, int right, int node, const vector<ll> &arr)
	{
		// If the segment has only one element, leaf node
		if (left == right)
		{
			if (left < (int)arr.size())
				seg[node] = Node(arr[left], dist(arr[left]));
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
		static const LazyNode none;
		if (!memcmp(&lazy[node], &none, sizeof none))
			return;
		const LazyNode tag = lazy[node];
		lazy[node] = none;
		if (tag.set != 0)
		{
			seg[node].val = tag.set + tag.add;
			seg[node].mn = dist(seg[node].val);
		}
		else
		{
			seg[node].mn -= tag.add;
			if (seg[node].val != 0)
				seg[node].val += tag.add;
		}
		// If the node is not a leaf, propagate the value
		if (left != right)
		{
			for (int c : {L, R})
				lazy[c] += tag;
		}
	}
	void fix(int left, int right, int node)
	{
		push(left, right, node);
		if (seg[node].mn >= 0)
			return;
		if (seg[node].val != 0)
		{
			seg[node].mn = dist(seg[node].val);
			return;
		}
		fix(left, mid, L);
		fix(mid + 1, right, R);
		seg[node] = merge(seg[L], seg[R]);
	}
	void assign(int left, int right, int node, int leftQuery, int rightQuery, const ll &val)
	{
		push(left, right, node);
		// If the range is invalid, return
		if (left > rightQuery || right < leftQuery)
			return;
		// If the range matches the segment
		if (left >= leftQuery && right <= rightQuery)
		{
			// Update the lazy value
			lazy[node] += LazyNode(val, 0);
			// Apply the update immediately
			push(left, right, node);
			return;
		}
		assign(left, mid, L, leftQuery, rightQuery, val);
		assign(mid + 1, right, R, leftQuery, rightQuery, val);
		seg[node] = merge(seg[L], seg[R]);
	}
	void add(int left, int right, int node, int leftQuery, int rightQuery, const ll &val)
	{
		push(left, right, node);
		// If the range is invalid, return
		if (left > rightQuery || right < leftQuery)
			return;
		// If the range matches the segment
		if (left >= leftQuery && right <= rightQuery)
		{
			lazy[node] += LazyNode(0, val);
			push(left, right, node);
			return;
		}
		add(left, mid, L, leftQuery, rightQuery, val);
		add(mid + 1, right, R, leftQuery, rightQuery, val);
		seg[node] = merge(seg[L], seg[R]);
	}
	Node query(int left, int right, int node, int idx)
	{
		// Apply the pending updates if any
		push(left, right, node);
		if (left == right)
			return seg[node];
		if (idx <= mid)
			return query(left, mid, L, idx);
		return query(mid + 1, right, R, idx);
	}

public:
	LazySegmentTree(const vector<ll> &arr)
	{
		pw.push_back(1);
		while (pw.back() <= (ll)1e18 / 42)
			pw.push_back(pw.back() * 42);
		pw.push_back(INF); // keeps lower_bound in dist valid for values above the last power
		size = 1;
		int n = arr.size();
		while (size < n)
			size <<= 1;
		seg = vector<Node>(2 * size);
		lazy = vector<LazyNode>(2 * size);
		build(0, size - 1, 0, arr);
	}
	void assign(int left, int right, const ll &val)
	{
		assign(0, size - 1, 0, left, right, val);
	}
	void add(int left, int right, const ll &val)
	{
		do
		{
			add(0, size - 1, 0, left, right, val);
			fix(0, size - 1, 0);
		} while (seg[0].mn == 0);
	}
	ll query(int idx)
	{
		Node ans = query(0, size - 1, 0, idx);
		return ans.val;
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
		ll N, Q;
		cin >> N >> Q;
		vector<ll> arr(N);
		for (int i = 0; i < N; i++)
			cin >> arr[i];
		LazySegmentTree segTree(arr);
		while (Q--)
		{
			ll query, L, R, val;
			cin >> query;
			if (query == 1)
			{
				cin >> L;
				cout << segTree.query(--L) << endl;
			}
			else
			{
				cin >> L >> R >> val;
				--L, --R;
				if (query == 2)
					segTree.assign(L, R, val);
				else
					segTree.add(L, R, val);
			}
		}
	}
	return 0;
}