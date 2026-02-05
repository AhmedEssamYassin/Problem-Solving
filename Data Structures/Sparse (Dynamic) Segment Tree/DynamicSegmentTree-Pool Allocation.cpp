#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

// Memory Pool (Static Allocation)
// Q updates * depth * number of trees ~= ... nodes.
const int MAX_NODES = 1.4e7;
struct Node
{
	int value;
	int L, R;
} tree[MAX_NODES];
int ptr = 0;

int newNode()
{
	++ptr;
	tree[ptr] = {0, 0, 0};
	return ptr;
}

struct DynamicSegmentTree
{
#define mid ((left + right) >> 1)
private:
	ll N;
	int root;
	void merge(int curr)
	{
		tree[curr].value = (tree[curr].L ? tree[tree[curr].L].value : 0) +
						   (tree[curr].R ? tree[tree[curr].R].value : 0);
	}
	void update(ll left, ll right, int &curr, ll idx, const ll &newValue)
	{
		// idx is not in range [left, right]
		if (left > idx || right < idx)
			return;
		if (!curr)
			curr = newNode();
		// curr is the Node that manage only ith element
		if (left == right)
		{
			tree[curr].value += newValue;
			return;
		}

		update(left, mid, tree[curr].L, idx, newValue);
		update(mid + 1, right, tree[curr].R, idx, newValue);
		merge(curr);
	}
	ll query(ll left, ll right, int curr, ll leftQuery, ll rightQuery)
	{
		// [left, right] doesn't intersect with [leftQuery, rightQuery]
		if (curr == 0 || left > rightQuery || right < leftQuery)
			return 0;

		// [left, curR] is inside [leftQuery, rightQuery]
		if (leftQuery <= left && right <= rightQuery)
			return tree[curr].value;

		ll leftSegment = query(left, mid, tree[curr].L, leftQuery, rightQuery);
		ll rightSegment = query(mid + 1, right, tree[curr].R, leftQuery, rightQuery);
		return (leftSegment + rightSegment);
	}

public:
	DynamicSegmentTree(ll rangeSize = 1e9)
	{
		root = newNode();
		N = rangeSize + 2;
	}
	void update(ll idx, const ll &val)
	{
		update(1, N, root, idx, val);
	}
	ll query(ll left, ll right)
	{
		ll ans = query(1, N, root, left, right);
		return ans;
	}
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
		vector<ll> vc(n);
		for (int i{}; i < n; i++)
			cin >> vc[i];
		vector<int> cntL(n);
		ptr = 0; // Reset pool
		DynamicSegmentTree segTree(1e9);
		for (int i = 0; i < n; i++)
		{
			cntL[i] = segTree.query(vc[i] + 1, 1e9);
			segTree.update(vc[i], 1);
		}

		ptr = 0; // Reset pool
		segTree = DynamicSegmentTree(1e9);
		ll cnt = 0;
		for (int j = n - 2; j > 0; j--)
		{
			segTree.update(vc[j + 1], 1);
			ll cntR = segTree.query(1, vc[j] - 1);
			cnt += cntL[j] * cntR;
		}

		cout << cnt << endl;
	}
	return 0;
}