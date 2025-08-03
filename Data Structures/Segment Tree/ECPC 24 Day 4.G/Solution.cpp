#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

const ll mod = 1e9 + 9;

#define double_size_t std::conditional_t<(mod > (1LL << 31)), __int128_t, long long>
inline ll add64(const ll &a, const ll &b)
{
	double_size_t res = double_size_t(a) + b;
	if (res >= mod)
		res -= mod;
	return res;
}

inline ll sub64(const ll &a, const ll &b)
{
	double_size_t res = double_size_t(a) - b;
	if (res < 0)
		res += mod;
	if (res >= mod)
		res -= mod;
	return res;
}

inline ll mult64(const ll &a, const ll &b)
{
	return double_size_t(a) * b % mod;
}

ll modPow(ll N, ll power)
{
	if (N % mod == 0 || N == 0)
		return 0;
	if (N == 1 || power == 0)
		return 1;

	ll res{1};
	while (power)
	{
		if (power & 1)
			res = mult64(res, N);
		N = mult64(N, N);
		power >>= 1;
	}
	return res;
}

struct SegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		ll sum;
		ll value;
		ll cnt;
		// Constructors
		Node() { sum = 0, value = 0, cnt = 0; }
		Node(ll val, ll i) : sum(val == 1 ? modPow(2, i) : 0), value(val), cnt(val > 1) {}
	};
	int size;
	vector<Node> seg;
	Node merge(const Node &leftNode, const Node &rightNode)
	{
		Node res;
		res.sum = add64(leftNode.sum, rightNode.sum);
		res.cnt = leftNode.cnt + rightNode.cnt;
		return res;
	}
	void build(int left, int right, int node, const vector<ll> &arr)
	{
		if (left == right)
		{
			if (left < arr.size())
				seg[node] = {arr[left], left};
			return;
		}
		// Building left node
		build(left, mid, L, arr);

		// Building right node
		build(mid + 1, right, R, arr);

		// Returning to parent nodes
		seg[node] = merge(seg[L], seg[R]);
	}
	void update(int left, int right, int node, int idx, const ll &val)
	{
		if (left == right)
		{
			seg[node] = {((seg[node].value ^ val) & 1), left};
			return;
		}
		if (idx <= mid)
			update(left, mid, L, idx, val);
		else
			update(mid + 1, right, R, idx, val);
		// Updating while returning to parent nodes
		seg[node] = merge(seg[L], seg[R]);
	}
	Node query(int left, int right, int node, int leftQuery, int rightQuery)
	{
		// Out of range
		if (right < leftQuery || left > rightQuery)
			return Node(); // A value that doesn't affect the minimization

		// The whole range is the answer
		if (left >= leftQuery && right <= rightQuery)
			return seg[node];

		// ONLY a part of this segment belongs to the query
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
		seg = vector<Node>(2 * size, Node());
		build(0, size - 1, 0, arr);
	}
	void update(int idx, const ll &val)
	{
		update(0, size - 1, 0, idx, val);
	}
	pair<ll, ll> query(int left, int right)
	{
		Node ans = query(0, size - 1, 0, left, right);
		return {ans.sum, ans.cnt};
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
	ll n, m;
	// cin >> t;
	while (t--)
	{
		cin >> n;
		vector<ll> vc(n);
		for (int i{}; i < n; i++)
			cin >> vc[i];
		reverse(vc.begin(), vc.end());
		SegmentTree segTree(vc);
		cin >> m;
		while (m--)
		{
			ll type, i, x, l, r;
			cin >> type;
			// 1 2 3 8 6
			// 6 8 3 2 1
			if (type == 1)
			{
				cin >> i >> x;
				--i;
				segTree.update(n - i - 1, x);
			}
			else
			{
				cin >> l >> r;
				// 1 3
				// 0 2
				// l = 4, r = 2
				--l, --r;
				l = n - l - 1;
				r = n - r - 1;
				auto [value, cnt] = segTree.query(r, l);
				if (cnt > 0)
					cout << "Impossible\n";
				else
				{
					value = mult64(value, modPow(modPow(2, r), mod - 2));
					cout << value << endl;
				}
			}
		}
	}
	return 0;
}