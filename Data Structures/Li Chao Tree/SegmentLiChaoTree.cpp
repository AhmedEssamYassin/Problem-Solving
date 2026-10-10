#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

/*
Segment Li Chao Tree (offline over the query coordinates)
add(l, r, a, b)    : add line y = a * x + b, active only for x in [l, r]   O(log^2 N)
query(x)           : max over active lines at x, LLONG_MIN if none         O(log N)
*/

struct SegmentLiChaoTree
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
	int size;
	vector<ll> xs;
	vector<Line> seg;
	void insert(int left, int right, int node, Line line)
	{
		while (true)
		{
			bool lb = line.eval(xs[left]) > seg[node].eval(xs[left]);
			bool mb = line.eval(xs[mid]) > seg[node].eval(xs[mid]);
			if (mb)
				swap(line, seg[node]);
			if (left == right || line.b == LLONG_MIN)
				return;
			if (lb != mb)
				node = L, right = mid;
			else
				node = R, left = mid + 1;
		}
	}
	void add(int left, int right, int node, int l, int r, const Line &line)
	{
		if (right < l || left > r)
			return;
		if (l <= left && right <= r)
			return insert(left, right, node, line);
		add(left, mid, L, l, r, line);
		add(mid + 1, right, R, l, r, line);
	}
	int idx(ll x) const { return lower_bound(xs.begin(), xs.end(), x) - xs.begin(); }

public:
	SegmentLiChaoTree(vector<ll> queryXs) : xs(move(queryXs))
	{
		sort(xs.begin(), xs.end());
		xs.erase(unique(xs.begin(), xs.end()), xs.end());
		if (xs.empty())
			xs.push_back(0);
		size = xs.size();
		seg.assign(2 * size, Line());
	}
	void add(ll l, ll r, ll a, ll b)
	{
		int lo = idx(l), hi = int(upper_bound(xs.begin(), xs.end(), r) - xs.begin()) - 1;
		if (lo <= hi)
			add(0, size - 1, 0, lo, hi, {a, b});
	}
	ll query(ll x) const
	{
		int i = idx(x), left = 0, right = size - 1, node = 0;
		assert(i < size && xs[i] == x);
		ll best = LLONG_MIN;
		while (true)
		{
			best = max(best, seg[node].eval(x));
			if (left == right)
				return best;
			if (i <= mid)
				node = L, right = mid;
			else
				node = R, left = mid + 1;
		}
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
		vector<array<ll, 5>> q(n);
		vector<ll> xs;
		for (auto &[tp, a, b, l, r] : q)
		{
			cin >> tp >> a;
			if (tp == 1)
				cin >> b >> l >> r, xs.push_back(l), xs.push_back(r);
			else
				xs.push_back(a);
		}
		SegmentLiChaoTree lct(xs);
		for (auto &[tp, a, b, l, r] : q)
		{
			if (tp == 1)
				lct.add(l, r, a, b);
			else
			{
				ll v = lct.query(a);
				if (v == LLONG_MIN)
					cout << "NO" << endl;
				else
					cout << v << endl;
			}
		}
	}
	return 0;
}
