#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"
const ll INF = 4e18;

// Two arrays A1, A2: range chmin / range add on one of them, query max(A1[i] + A2[i]) over a range
struct SegmentTreeBeats
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		ll mx[2] = {-INF, -INF}, se[2] = {-INF, -INF}; // max, strict second max per array
		// best[x][y]: max A1 + A2 over positions of class x in A1 and class y in A2 (class 0: the max elements, 1: the rest)
		ll best[2][2] = {{-INF, -INF}, {-INF, -INF}};
		Node() {}
		Node(const ll &A, const ll &B) { mx[0] = A, mx[1] = B, best[0][0] = A + B; }
		bool empty() const { return mx[0] == -INF; }
		ll answer() const { return max({best[0][0], best[0][1], best[1][0], best[1][1]}); }
	};
	struct LazyNode
	{
		ll add[2][2] = {}; // add[a][k]: added to array a, k = 0 max elements / 1 the rest
		LazyNode &operator+=(const LazyNode &RHS)
		{
			for (int a = 0; a < 2; a++)
				for (int k = 0; k < 2; k++)
					add[a][k] += RHS.add[a][k];
			return *this;
		}
	};
	int size;
	vector<Node> seg;
	vector<LazyNode> lazy;
	Node merge(const Node &a, const Node &b)
	{
		Node res;
		for (int i = 0; i < 2; i++)
			res.mx[i] = max(a.mx[i], b.mx[i]);
		for (const Node *c : {&a, &b})
		{
			for (int i = 0; i < 2; i++)
				res.se[i] = max(res.se[i], c->mx[i] == res.mx[i] ? c->se[i] : c->mx[i]);
			for (int x = 0; x < 2; x++)
			{
				for (int y = 0; y < 2; y++)
				{
					if (c->best[x][y] != -INF)
					{
						// a child's max elements stay max elements only if the child holds the node's max
						int px = x == 0 && c->mx[0] == res.mx[0] ? 0 : 1;
						int py = y == 0 && c->mx[1] == res.mx[1] ? 0 : 1;
						res.best[px][py] = max(res.best[px][py], c->best[x][y]);
					}
				}
			}
		}
		return res;
	}
	void build(int left, int right, int node, const vector<ll> &arr1, const vector<ll> &arr2)
	{
		if (left == right)
		{
			if (left < (int)arr1.size())
				seg[node] = Node(arr1[left], arr2[left]);
			return;
		}
		build(left, mid, L, arr1, arr2);
		build(mid + 1, right, R, arr1, arr2);
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
		const ll oldMx[2] = {cur.mx[0], cur.mx[1]};
		for (int x = 0; x < 2; x++)
			for (int y = 0; y < 2; y++)
				if (cur.best[x][y] != -INF)
					cur.best[x][y] += tag.add[0][x] + tag.add[1][y];
		for (int a = 0; a < 2; a++)
		{
			cur.mx[a] += tag.add[a][0];
			if (cur.se[a] != -INF)
				cur.se[a] += tag.add[a][1];
		}
		if (left != right)
		{
			for (int c : {L, R})
			{
				if (!seg[c].empty())
				{
					for (int a = 0; a < 2; a++)
					{
						bool top = seg[c].mx[a] + lazy[c].add[a][0] == oldMx[a];
						lazy[c].add[a][0] += tag.add[a][top ? 0 : 1];
						lazy[c].add[a][1] += tag.add[a][1];
					}
				}
			}
		}
	}
	void chmin(int left, int right, int node, int leftQuery, int rightQuery, int arr, const ll &val)
	{
		push(left, right, node);
		if (left > rightQuery || right < leftQuery || seg[node].mx[arr] <= val)
			return;
		if (left >= leftQuery && right <= rightQuery && seg[node].se[arr] < val)
		{
			lazy[node].add[arr][0] += val - seg[node].mx[arr];
			push(left, right, node);
			return;
		}
		chmin(left, mid, L, leftQuery, rightQuery, arr, val);
		chmin(mid + 1, right, R, leftQuery, rightQuery, arr, val);
		seg[node] = merge(seg[L], seg[R]);
	}
	void add(int left, int right, int node, int leftQuery, int rightQuery, int arr, const ll &val)
	{
		push(left, right, node);
		if (left > rightQuery || right < leftQuery)
			return;
		if (left >= leftQuery && right <= rightQuery)
		{
			lazy[node].add[arr][0] += val, lazy[node].add[arr][1] += val;
			push(left, right, node);
			return;
		}
		add(left, mid, L, leftQuery, rightQuery, arr, val);
		add(mid + 1, right, R, leftQuery, rightQuery, arr, val);
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
	SegmentTreeBeats(const vector<ll> &arr1, const vector<ll> &arr2)
	{
		size = 1;
		while (size < (int)arr1.size())
			size <<= 1;
		seg.resize(2 * size), lazy.resize(2 * size);
		build(0, size - 1, 0, arr1, arr2);
	}
	// A[arr][i] = min(A[arr][i], val) for i in [left, right]
	void chmin(int arr, int left, int right, const ll &val) { chmin(0, size - 1, 0, left, right, arr, val); }
	// A[arr][i] += val for i in [left, right]
	void add(int arr, int left, int right, const ll &val) { add(0, size - 1, 0, left, right, arr, val); }
	// max(A1[i] + A2[i]) for i in [left, right]
	ll query(int left, int right) { return query(0, size - 1, 0, left, right).answer(); }

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
		// 1 A L R val : A_a[i] = min(A_a[i], val) for i in [L, R]   (A is 1 or 2)
		// 2 A L R val : A_a[i] += val for i in [L, R]               (A is 1 or 2)
		// 3 L R       : print max(A1[i] + A2[i]) for i in [L, R]
		ll N, M;
		cin >> N >> M;
		vector<ll> arr1(N), arr2(N);
		for (int i = 0; i < N; i++)
			cin >> arr1[i];
		for (int i = 0; i < N; i++)
			cin >> arr2[i];
		SegmentTreeBeats segTree(arr1, arr2);

		while (M--)
		{
			ll query, A, L, R, val;
			cin >> query;
			if (query == 3)
			{
				cin >> L >> R;
				cout << segTree.query(L, R) << endl;
			}
			else
			{
				cin >> A >> L >> R >> val;
				if (query == 1)
					segTree.chmin(A - 1, L, R, val);
				else
					segTree.add(A - 1, L, R, val);
			}
		}
	}
	return 0;
}
