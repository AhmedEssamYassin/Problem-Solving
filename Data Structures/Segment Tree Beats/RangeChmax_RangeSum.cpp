#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"
const ll INF = 4e18;

// Range chmax, range sum
struct SegmentTreeBeats
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		ll mn = INF, se = INF, sum = 0; // min, strict second min, sum
		int cnt = 0, len = 0;           // min count, real elements
		Node() {}
		Node(const ll &N) : mn(N), sum(N), cnt(1), len(1) {}
	};
	struct LazyNode
	{
		ll addMn = 0, addOth = 0; // add to the min elements / the others
		LazyNode &operator+=(const LazyNode &RHS)
		{
			addMn += RHS.addMn, addOth += RHS.addOth;
			return *this;
		}
	};
	int size;
	vector<Node> seg;
	vector<LazyNode> lazy;
	Node merge(const Node &a, const Node &b)
	{
		Node res;
		res.len = a.len + b.len, res.sum = a.sum + b.sum;
		res.mn = min(a.mn, b.mn);
		for (const Node *c : {&a, &b})
		{
			if (c->mn == res.mn)
				res.cnt += c->cnt, res.se = min(res.se, c->se);
			else
				res.se = min(res.se, c->mn);
		}
		return res;
	}
	void build(int left, int right, int node, const vector<ll> &arr)
	{
		if (left == right)
		{
			if (left < (int)arr.size())
				seg[node] = arr[left];
			return;
		}
		build(left, mid, L, arr);
		build(mid + 1, right, R, arr);
		seg[node] = merge(seg[L], seg[R]);
	}
	void push(int left, int right, int node)
	{
		static const LazyNode none{};
		if (!memcmp(&lazy[node], &none, sizeof none))
			return;
		const LazyNode tag = lazy[node];
		lazy[node] = none;
		Node &cur = seg[node];
		const ll oldMn = cur.mn;
		cur.sum += tag.addMn * cur.cnt + tag.addOth * (cur.len - cur.cnt);
		cur.mn += tag.addMn;
		if (cur.se != INF)
			cur.se += tag.addOth;
		if (left != right)
		{
			for (int c : {L, R})
				if (seg[c].len)
					lazy[c] += (seg[c].mn + lazy[c].addMn == oldMn) ? tag : LazyNode{tag.addOth, tag.addOth};
		}
	}
	void chmax(int left, int right, int node, int leftQuery, int rightQuery, const ll &val)
	{
		push(left, right, node);
		if (left > rightQuery || right < leftQuery || seg[node].mn >= val)
			return;
		if (left >= leftQuery && right <= rightQuery && seg[node].se > val)
		{
			lazy[node] += LazyNode{val - seg[node].mn, 0};
			push(left, right, node);
			return;
		}
		chmax(left, mid, L, leftQuery, rightQuery, val);
		chmax(mid + 1, right, R, leftQuery, rightQuery, val);
		seg[node] = merge(seg[L], seg[R]);
	}
	Node query(int left, int right, int node, int leftQuery, int rightQuery)
	{
		push(left, right, node);
		if (left > rightQuery || right < leftQuery)
			return Node();
		if (left >= leftQuery && right <= rightQuery)
			return seg[node];
		return merge(query(left, mid, L, leftQuery, rightQuery), query(mid + 1, right, R, leftQuery, rightQuery));
	}

public:
	SegmentTreeBeats(const vector<ll> &arr)
	{
		size = 1;
		while (size < (int)arr.size())
			size <<= 1;
		seg.resize(2 * size), lazy.resize(2 * size);
		build(0, size - 1, 0, arr);
	}
	void chmax(int left, int right, const ll &val) { chmax(0, size - 1, 0, left, right, val); }
	ll query(int left, int right) { return query(0, size - 1, 0, left, right).sum; }
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
		// 1 L R val : a[i] = max(a[i], val) for i in [L, R]
		// 2 L R     : print the sum of a[i] for i in [L, R]
		ll N, M;
		cin >> N >> M;
		vector<ll> arr(N);
		for (int i = 0; i < N; i++)
			cin >> arr[i];
		SegmentTreeBeats segTree(arr);

		while (M--)
		{
			ll query, L, R, val;
			cin >> query >> L >> R;
			if (query == 1)
			{
				cin >> val;
				segTree.chmax(L, R, val);
			}
			else
				cout << segTree.query(L, R) << endl;
		}
	}
	return 0;
}
