#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"
const ll INF = 4e18;

// Range chmax, range add (val may be negative), range sum of B
// After every update, for all i: B[i] = min(B[i], A[i]), B starts as A
//
// Keep D = B - A (always <= 0), so sum B = sum A + sum D
// If an update changes A[i] by a, then D[i] = min(D[i] - a, 0)
//
// A is a beats tree (min, strict second min). Each node splits its elements into two classes:
// the min elements of A (class 0) and the rest (class 1)
// - chmax(val): when min < val < second min only the min elements change, by a = val - min > 0, so D never caps
// - add(val): every element changes by val, D caps only when val < 0
// Within each class D keeps (max D, strict second max D, count of max D)
// "D -= a, then cap at 0" is a valid tag when only the max D elements can exceed 0 (they become exactly 0)
// Otherwise the update goes deeper, which happens only for add with val < 0
struct SegmentTreeBeats
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	struct Stat
	{
		ll mx = -INF, se = -INF, sum = 0; // max D, strict second max D, sum of D
		int cnt = 0, num = 0;             // max count, elements
		Stat() {}
		Stat(const ll &N) : mx(N), sum(N), cnt(1), num(1) {}
	};
	Stat mergeStat(const Stat &a, const Stat &b)
	{
		Stat res;
		res.num = a.num + b.num, res.sum = a.sum + b.sum;
		res.mx = max(a.mx, b.mx);
		for (const Stat *c : {&a, &b})
			if (c->mx == res.mx)
				res.cnt += c->cnt, res.se = max(res.se, c->se);
			else
				res.se = max(res.se, c->mx);
		return res;
	}
	struct Node
	{
		ll mn = INF, se = INF, sumA = 0; // min of A, strict second min of A, sum of A
		Stat d[2];                       // D per class: [0] min elements of A, [1] the rest
		Node() {}
		Node(const ll &N) : mn(N), sumA(N) { d[0] = Stat(0); } // B = A initially, so D = 0
		ll sumB() const { return sumA + d[0].sum + d[1].sum; }
	};
	struct LazyNode
	{
		// per class of A: [0] min elements, [1] the rest
		ll addA[2] = {}, dMx[2] = {}, dOth[2] = {}; // added to A / to the max D / to the other D
		LazyNode &operator+=(const LazyNode &RHS)
		{
			for (int k = 0; k < 2; k++)
				addA[k] += RHS.addA[k], dMx[k] += RHS.dMx[k], dOth[k] += RHS.dOth[k];
			return *this;
		}
	};
	int size;
	vector<Node> seg;
	vector<LazyNode> lazy;
	Node merge(const Node &a, const Node &b)
	{
		Node res;
		res.sumA = a.sumA + b.sumA;
		res.mn = min(a.mn, b.mn);
		for (const Node *c : {&a, &b})
		{
			// a child's min elements stay in class 0 only if the child holds the node's min of A
			int k = c->mn == res.mn ? 0 : 1;
			res.se = min(res.se, k ? c->mn : c->se);
			res.d[k] = mergeStat(res.d[k], c->d[0]);
			res.d[1] = mergeStat(res.d[1], c->d[1]);
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
		const Node old = cur;
		cur.sumA += tag.addA[0] * cur.d[0].num + tag.addA[1] * cur.d[1].num;
		cur.mn += tag.addA[0];
		if (cur.se != INF)
			cur.se += tag.addA[1];
		for (int k = 0; k < 2; k++)
		{
			Stat &s = cur.d[k];
			if (!s.num)
				continue;
			s.sum += tag.dMx[k] * s.cnt + tag.dOth[k] * (s.num - s.cnt);
			s.mx += tag.dMx[k];
			if (s.se != -INF)
				s.se += tag.dOth[k];
		}
		if (left != right)
		{
			for (int c : {L, R})
			{
				LazyNode t;
				for (int j = 0; j < 2; j++)
				{
					if (seg[c].d[j].num)
					{
						// class of the group within the node, the child's min includes its pending tag
						int k = j == 0 && seg[c].mn + lazy[c].addA[0] == old.mn ? 0 : 1;
						// only the group holding the node's max D gets the max-D tag
						t.addA[j] = tag.addA[k];
						t.dMx[j] = seg[c].d[j].mx + lazy[c].dMx[j] == old.d[k].mx ? tag.dMx[k] : tag.dOth[k];
						t.dOth[j] = tag.dOth[k];
					}
				}
				lazy[c] += t;
			}
		}
	}
	// "add val" fits in a tag if, after D - val, at most the max D elements of each class exceed 0
	bool canAdd(int node, const ll &val)
	{
		for (const Stat &s : seg[node].d)
			if (s.num && s.mx > val && s.se != -INF && s.se >= val)
				return false;
		return true;
	}
	void chmax(int left, int right, int node, int leftQuery, int rightQuery, const ll &val)
	{
		push(left, right, node);
		if (left > rightQuery || right < leftQuery || seg[node].mn >= val)
			return;
		if (left >= leftQuery && right <= rightQuery && seg[node].se > val)
		{
			ll a = val - seg[node].mn;
			LazyNode t;
			t.addA[0] = a, t.dMx[0] = t.dOth[0] = -a;
			lazy[node] += t;
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
		if (left >= leftQuery && right <= rightQuery && canAdd(node, val))
		{
			LazyNode t;
			t.addA[0] = t.addA[1] = val;
			for (int k = 0; k < 2; k++)
				if (const Stat &s = seg[node].d[k]; s.num)
					t.dOth[k] = -val, t.dMx[k] = s.mx > val ? -s.mx : -val; // a max D that would exceed 0 becomes 0
			lazy[node] += t;
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
	void add(int left, int right, const ll &val) { add(0, size - 1, 0, left, right, val); }
	ll query(int left, int right) { return query(0, size - 1, 0, left, right).sumB(); }

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
		// After every update, for all i: B[i] = min(B[i], A[i]), B starts as A
		// 1 L R val : a[i] = max(a[i], val) for i in [L, R]
		// 2 L R val : a[i] += val for i in [L, R]
		// 3 L R     : print the sum of B[i] (historic min) for i in [L, R]
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
			if (query == 3)
				cout << segTree.query(L, R) << endl;
			else
			{
				cin >> val;
				if (query == 1)
					segTree.chmax(L, R, val);
				else
					segTree.add(L, R, val);
			}
		}
	}
	return 0;
}
