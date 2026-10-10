#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"
const ll INF = 4e18;

// Range chmax, range chmin, range add, range sum of B
// Every operation does B[i]++ for each a[i] it changes, B starts as A
struct SegmentTreeBeats
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		ll mn = INF, smn = INF, mx = -INF, smx = -INF, sumB = 0; // min, strict second min, max, strict second max, sum of B
		int cntMn = 0, cntMx = 0, len = 0;
		Node() {}
		Node(const ll &N) : mn(N), mx(N), sumB(N), cntMn(1), cntMx(1), len(1) {}
	};
	struct LazyNode
	{
		// per class: [0] min, [1] max, [2] other (classes 0 and 1 carry the same values when all are equal)
		ll add[3] = {}, chg[3] = {}; // added to a[i] / to B[i]
		LazyNode &operator+=(const LazyNode &RHS)
		{
			for (int i = 0; i < 3; i++)
				add[i] += RHS.add[i], chg[i] += RHS.chg[i];
			return *this;
		}
	};
	int size;
	vector<Node> seg;
	vector<LazyNode> lazy;
	Node merge(const Node &a, const Node &b)
	{
		Node res;
		res.len = a.len + b.len, res.sumB = a.sumB + b.sumB;
		res.mn = min(a.mn, b.mn), res.mx = max(a.mx, b.mx);
		for (const Node *c : {&a, &b})
		{
			if (c->mn == res.mn)
				res.cntMn += c->cntMn, res.smn = min(res.smn, c->smn);
			else
				res.smn = min(res.smn, c->mn);
			if (c->mx == res.mx)
				res.cntMx += c->cntMx, res.smx = max(res.smx, c->smx);
			else
				res.smx = max(res.smx, c->mx);
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
		const ll oldMn = cur.mn, oldMx = cur.mx;
		if (cur.mn == cur.mx)
			cur.sumB += tag.chg[0] * cur.len;
		else
			cur.sumB += tag.chg[0] * cur.cntMn + tag.chg[1] * cur.cntMx + tag.chg[2] * (cur.len - cur.cntMn - cur.cntMx);
		cur.mn += tag.add[0], cur.mx += tag.add[1];
		// with two distinct values, the second min is the max and the second max is the min
		if (cur.smn != INF)
			cur.smn += (cur.smn == oldMx) ? tag.add[1] : tag.add[2];
		if (cur.smx != -INF)
			cur.smx += (cur.smx == oldMn) ? tag.add[0] : tag.add[2];
		if (left != right)
		{
			for (int c : {L, R})
			{
				if (seg[c].len)
				{
					ll cmn = seg[c].mn + lazy[c].add[0], cmx = seg[c].mx + lazy[c].add[1];
					int kMn = cmn == oldMn ? 0 : cmn == oldMx ? 1
					                                          : 2;
					int kMx = cmx == oldMn ? 0 : cmx == oldMx ? 1
					                                          : 2;
					lazy[c] += LazyNode{{tag.add[kMn], tag.add[kMx], tag.add[2]}, {tag.chg[kMn], tag.chg[kMx], tag.chg[2]}};
				}
			}
		}
	}
	void chmin(int left, int right, int node, int leftQuery, int rightQuery, const ll &val)
	{
		push(left, right, node);
		if (left > rightQuery || right < leftQuery || seg[node].mx <= val)
			return;
		if (left >= leftQuery && right <= rightQuery && seg[node].smx < val)
		{
			ll d = val - seg[node].mx;
			lazy[node] += seg[node].mn == seg[node].mx ? LazyNode{{d, d, 0}, {1, 1, 0}} : LazyNode{{0, d, 0}, {0, 1, 0}};
			push(left, right, node);
			return;
		}
		chmin(left, mid, L, leftQuery, rightQuery, val);
		chmin(mid + 1, right, R, leftQuery, rightQuery, val);
		seg[node] = merge(seg[L], seg[R]);
	}
	void chmax(int left, int right, int node, int leftQuery, int rightQuery, const ll &val)
	{
		push(left, right, node);
		if (left > rightQuery || right < leftQuery || seg[node].mn >= val)
			return;
		if (left >= leftQuery && right <= rightQuery && seg[node].smn > val)
		{
			ll d = val - seg[node].mn;
			lazy[node] += seg[node].mn == seg[node].mx ? LazyNode{{d, d, 0}, {1, 1, 0}} : LazyNode{{d, 0, 0}, {1, 0, 0}};
			push(left, right, node);
			return;
		}
		chmax(left, mid, L, leftQuery, rightQuery, val);
		chmax(mid + 1, right, R, leftQuery, rightQuery, val);
		seg[node] = merge(seg[L], seg[R]);
	}
	void add(int left, int right, int node, int leftQuery, int rightQuery, const ll &val)
	{
		push(left, right, node);
		if (left > rightQuery || right < leftQuery)
			return;
		if (left >= leftQuery && right <= rightQuery)
		{
			lazy[node] += LazyNode{{val, val, val}, {1, 1, 1}};
			push(left, right, node);
			return;
		}
		add(left, mid, L, leftQuery, rightQuery, val);
		add(mid + 1, right, R, leftQuery, rightQuery, val);
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
	void chmin(int left, int right, const ll &val) { chmin(0, size - 1, 0, left, right, val); }
	void add(int left, int right, const ll &val) { add(0, size - 1, 0, left, right, val); }
	ll query(int left, int right) { return query(0, size - 1, 0, left, right).sumB; }

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
		// 2 L R val : a[i] = min(a[i], val) for i in [L, R]
		// 3 L R val : a[i] += val for i in [L, R]
		// 4 L R     : print the sum of B[i] for i in [L, R]
		// Every operation does B[i]++ for each a[i] it actually changes, B starts as A
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
			if (query == 4)
				cout << segTree.query(L, R) << endl;
			else
			{
				cin >> val;
				if (query == 1)
					segTree.chmax(L, R, val);
				else if (query == 2)
					segTree.chmin(L, R, val);
				else
					segTree.add(L, R, val);
			}
		}
	}
	return 0;
}
