#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

function<uint64_t()> random_address = []() -> uint64_t
{
	char *p = new char;
	delete p;
	return uint64_t(p);
};
const uint64_t SEED = chrono::steady_clock::now().time_since_epoch().count() * (random_address() | 1);
std::mt19937 rnd(SEED);
#define rng(l, r) uniform_int_distribution<int64_t>(l, r)(rnd)

vector<ll> XOR;
vector<ll> pref;
void preCompute(int N)
{
	XOR.assign(N + 1, 0);
	pref.assign(N + 1, 0);
	for (ll &hash : XOR)
		hash = rng(1LL << 32, 1LL << 63);
	pref[0] = XOR[0];
	for (int i{1}; i <= N; i++)
		pref[i] = (pref[i - 1] ^ XOR[i]);
}
static int autoCall = (preCompute(1e5), 0);

struct SegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		ll hash;
		ll minValue;
		// Constructors
		Node() {}
		Node(const ll &a, const ll &b) : hash(a), minValue(b) {}
	};
	int size;
	vector<Node> seg;
	Node merge(const Node &leftNode, const Node &rightNode)
	{
		Node res;
		res.hash = (leftNode.hash ^ rightNode.hash);
		res.minValue = min(leftNode.minValue, rightNode.minValue);
		return res;
	}
	void build(int left, int right, int node, const vector<ll> &arr)
	{
		if (left == right) // Leaf Node (single element)
		{
			if (left < arr.size()) // Making sure we are inside the boundaries of the array
				seg[node] = {XOR[arr[left]], arr[left]};
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
			seg[node] = {XOR[val], val};
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
			return {0, LLONG_MAX}; // A value that doesn't affect the query

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
		seg = vector<Node>(2 * size, {0, LLONG_MAX});
		build(0, size - 1, 0, arr);
	}
	void update(int idx, const ll &val)
	{
		update(0, size - 1, 0, idx, val);
	}
	pair<ll, ll> query(int left, int right)
	{
		Node ans = query(0, size - 1, 0, left, right);
		return {ans.hash, ans.minValue};
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
	ll N, Q;
	cin >> t;
	while (t--)
	{
		cin >> N >> Q;
		vector<ll> vc(N);
		for (int i{}; i < N; i++)
			cin >> vc[i];

		SegmentTree segTree(vc);
		for (int i{}; i < Q; i++)
		{
			ll type, x, y, L, R;
			cin >> type;
			if (type == 1)
			{
				cin >> x >> y;
				x--; // To be 0-based
				segTree.update(x, y);
			}
			else
			{
				cin >> L >> R;
				L--, R--; // To be 0-based
				auto [rangeHash, minValue] = segTree.query(L, R);
				ll len = R - L + 1;
				ll a = minValue;
				ll b = a + len - 1;
				ll expectedHash = (a == 0 ? pref[b] : pref[b] ^ pref[a - 1]);
				if (rangeHash == expectedHash)
					cout << "YES\n";
				else
					cout << "NO\n";
			}
		}
	}
	return 0;
}