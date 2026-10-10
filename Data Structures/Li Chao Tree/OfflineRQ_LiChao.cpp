#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

/*
Offline Dynamic CHT (insert / delete lines, max query)
Segment tree over the query timeline: a line alive for queries [l, r] is stored in O(log Q) nodes.
DFS over that tree inserts each node's lines into a Li Chao tree and rolls the changes back on exit.
add(l, r, a, b)    : line y = a * x + b alive for the queries with index in [l, r]
solve()            : answer of every query, LLONG_MIN if no line is alive         O((N + Q) log Q log X)
*/

struct OfflineCHT
{
#define L (node + 1)
#define R (node + ((mid - left + 1) << 1))
#define mid ((left + right) >> 1)
private:
	struct Line
	{
		ll a = 0, b = LLONG_MIN;
		ll eval(ll x) const { return a * x + b; }
	};
	int Q;
	vector<ll> qx, xs, res;
	vector<Line> seg, ln;       // seg: Li Chao tree over xs, ln: all inserted lines
	vector<int> head, to, nxt;  // timeline tree buckets as linked lists of line ids
	vector<pair<int, Line>> jr; // rollback journal
	// Li Chao insert, every overwritten node is journaled
	void insert(int left, int right, int node, Line line)
	{
		while (true)
		{
			bool lb = line.eval(xs[left]) > seg[node].eval(xs[left]);
			bool mb = line.eval(xs[mid]) > seg[node].eval(xs[mid]);
			if (mb)
				jr.push_back({node, seg[node]}), swap(line, seg[node]);
			if (left == right || line.b == LLONG_MIN)
				return;
			if (lb != mb)
				node = L, right = mid;
			else
				node = R, left = mid + 1;
		}
	}
	ll query(int i) const
	{
		int left = 0, right = xs.size() - 1, node = 0;
		ll best = LLONG_MIN;
		while (true)
		{
			best = max(best, seg[node].eval(xs[i]));
			if (left == right)
				return best;
			if (i <= mid)
				node = L, right = mid;
			else
				node = R, left = mid + 1;
		}
	}
	void add(int left, int right, int node, int l, int r, int id)
	{
		if (right < l || left > r)
			return;
		if (l <= left && right <= r)
		{
			to.push_back(id), nxt.push_back(head[node]), head[node] = to.size() - 1;
			return;
		}
		add(left, mid, L, l, r, id);
		add(mid + 1, right, R, l, r, id);
	}
	void dfs(int left, int right, int node)
	{
		size_t m = jr.size();
		for (int e = head[node]; ~e; e = nxt[e])
			insert(0, xs.size() - 1, 0, ln[to[e]]);
		if (left == right)
			res[left] = query(lower_bound(xs.begin(), xs.end(), qx[left]) - xs.begin());
		else
			dfs(left, mid, L), dfs(mid + 1, right, R);
		while (jr.size() > m)
			seg[jr.back().first] = jr.back().second, jr.pop_back();
	}

public:
	OfflineCHT(vector<ll> queryXs) : Q(queryXs.size()), qx(queryXs), xs(move(queryXs)), res(Q), head(2 * max(Q, 1), -1)
	{
		sort(xs.begin(), xs.end());
		xs.erase(unique(xs.begin(), xs.end()), xs.end());
		if (xs.empty())
			xs.push_back(0);
		seg.assign(2 * xs.size(), Line());
	}
	void add(int l, int r, ll a, ll b)
	{
		if (l <= r)
			ln.push_back({a, b}), add(0, Q - 1, 0, l, r, ln.size() - 1);
	}
	vector<ll> solve()
	{
		if (Q)
			dfs(0, Q - 1, 0);
		return res;
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
		int n, j, tp;
		cin >> n;
		vector<ll> qs, A(n + 1), B(n + 1);
		vector<int> S(n + 1, -1); // S[i] = #queries before line i was added, -1 if i is not a live line
		vector<array<ll, 4>> rg;  // {l, r, a, b}
		for (int i = 1; i <= n; i++)
		{
			cin >> tp;
			if (tp == 1)
				cin >> A[i] >> B[i], S[i] = qs.size();
			else if (tp == 2)
				cin >> j, rg.push_back({S[j], (ll)qs.size() - 1, A[j], B[j]}), S[j] = -1;
			else
				cin >> A[i], qs.push_back(A[i]);
		}
		for (int i = 1; i <= n; i++)
			if (S[i] >= 0)
				rg.push_back({S[i], (ll)qs.size() - 1, A[i], B[i]});
		OfflineCHT cht(qs);
		for (auto &[l, r, a, b] : rg)
			cht.add(l, r, a, b);
		auto ans = cht.solve();
		for (int i = 0; i < (int)qs.size(); i++)
		{
			if (ans[i] == LLONG_MIN)
				cout << "EMPTY SET" << endl;
			else
				cout << ans[i] << endl;
		}
	}
	return 0;
}
