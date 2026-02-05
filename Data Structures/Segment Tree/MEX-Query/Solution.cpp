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
		ll mn;
		// Constructors
		Node() { mn = LLONG_MAX; }
		Node(ll val) : mn(val) {}
	};
	int size;
	vector<Node> seg;
	Node merge(const Node &leftNode, const Node &rightNode)
	{
		Node res;
		res.mn = min(leftNode.mn, rightNode.mn);
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
	void update(int left, int right, int node, int idx, const ll &x)
	{
		if (left == right)
		{
			seg[node] = x;
			return;
		}
		if (idx <= mid)
			update(left, mid, L, idx, x);
		else
			update(mid + 1, right, R, idx, x);
		// Updating while returning to parent nodes
		seg[node] = merge(seg[L], seg[R]);
	}
	Node query(int left, int right, int node, int x)
	{
		if (left == right)
			return left;
		if (seg[L].mn >= x)
			return query(mid + 1, right, R, x);
		else
			return query(left, mid, L, x);
	}

public:
	SegmentTree(const vector<ll> &arr)
	{
		size = 1;
		int n = arr.size();
		while (size < n)
			size <<= 1;
		seg = vector<Node>(2 * size);
		build(0, size - 1, 0, arr);
	}
	void update(int idx, const ll &val)
	{
		update(0, size - 1, 0, idx, val);
	}
	ll query(int x)
	{
		Node ans = query(0, size - 1, 0, x);
		return ans.mn;
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
		int n;
		cin >> n;
		vector<int> a(n + 1);
		for (int i = 1; i <= n; i++)
			cin >> a[i];
		int q;
		cin >> q;
		vector<vector<pair<int, int>>> queries(n + 1);
		for (int i = 0; i < q; i++)
		{
			int l, r;
			cin >> l >> r;
			queries[r].push_back({l, i});
		}

		vector<int> res(q);
		SegmentTree segTree(vector<ll>(n + 1, 0));
		for (int i = 1; i <= n; i++)
		{
			segTree.update(a[i], i);
			for (const auto &[l, ind] : queries[i])
				res[ind] = segTree.query(l);
		}
		for (const int &x : res)
			cout << x << endl;
	}
	return 0;
}