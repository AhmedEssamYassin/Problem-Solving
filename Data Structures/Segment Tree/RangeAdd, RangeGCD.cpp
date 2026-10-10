#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

class FenwickTree
{
private:
	int N;
	vector<ll> BIT;

public:
	FenwickTree(int sz)
	{
		N = sz;
		BIT.assign(N + 1, 0);
	}
	void update(int pos, const int &delta)
	{
		for (int i = pos; i <= N; i |= (i + 1))
			BIT[i] += delta;
	}

	ll query(int pos)
	{
		ll sum = 0;
		for (int i = pos; i >= 0; i = (i & (i + 1)) - 1)
			sum += BIT[i];
		return sum;
	}

	ll rangeQuery(int L, int R)
	{
		if (L > R)
			return 0; // Neutral value
		return (query(R) - query(L - 1));
	}
};

struct SegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		ll g;
		Node() { g = 0; }
		Node(ll val) { g = val; }
	};
	int size;
	vector<Node> seg;
	Node merge(const Node &leftNode, const Node &rightNode)
	{
		Node res;
		res.g = gcd(leftNode.g, rightNode.g);
		return res;
	}
	void build(int left, int right, int node, const vector<ll> &arr)
	{
		if (left == right)
		{
			if (left < arr.size())
				seg[node] = arr[left];
			return;
		}
		build(left, mid, L, arr);
		build(mid + 1, right, R, arr);
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
		seg[node] = merge(seg[L], seg[R]);
	}
	Node query(int left, int right, int node, int leftQuery, int rightQuery)
	{
		if (right < leftQuery || left > rightQuery)
			return Node(); // A neutral value that doesn't affect other queries

		if (left >= leftQuery && right <= rightQuery)
			return seg[node];

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
		seg = vector<Node>(2 * size);
		build(0, size - 1, 0, arr);
	}
	void update(int idx, const ll &val)
	{
		update(0, size - 1, 0, idx, val);
	}
	ll query(int left, int right)
	{
		Node ans = query(0, size - 1, 0, left, right);
		return ans.g;
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
		int n, q;
		cin >> n >> q;
		vector<ll> vc(n), D(n + 1);
		for (int i{}; i < n; i++)
			cin >> vc[i];
		D[0] = vc[0];
		for (int i = 1; i < n; i++)
			D[i] = vc[i] - vc[i - 1];
		FenwickTree bit(n);
		SegmentTree segTree(D);
		auto slv = [&](int l, int r)
		{
			ll f = vc[l] + bit.query(l);
			if (l == r)
				return abs(f);
			return gcd(f, segTree.query(l + 1, r));
		};
		while (q--)
		{
			int tp, l, r, x;
			cin >> tp;
			if (tp == 1)
			{
				cin >> l >> r >> x;
				--l, --r;
				bit.update(l, x);
				bit.update(r + 1, -x);
				segTree.update(l, D[l] += x);
				segTree.update(r + 1, D[r + 1] -= x);
			}
			else
			{
				cin >> l >> r;
				--l, --r;
				cout << slv(l, r) << endl;
			}
		}
	}
	return 0;
}
