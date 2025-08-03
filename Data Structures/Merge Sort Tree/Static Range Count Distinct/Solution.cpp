#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct MergeSortSegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		vector<ll> range;
		// Constructors
		Node() {}
		Node(const ll &value) { range.push_back(value); }
	};
	int size;
	vector<Node> seg;
	Node merge(const Node &leftNode, const Node &rightNode)
	{
		Node res;
		res.range.resize(leftNode.range.size() + rightNode.range.size());
		std::merge(leftNode.range.begin(), leftNode.range.end(), rightNode.range.begin(), rightNode.range.end(), res.range.begin());
		return res;
	}
	void build(int left, int right, int node, const vector<ll> &arr)
	{
		if (left == right) // Leaf Node (single element)
		{
			if (left < arr.size()) // Making sure we are inside the boundaries of the array
				seg[node] = arr[left];
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
			seg[node] = val;
			return;
		}
		if (idx <= mid)
			update(left, mid, L, idx, val);
		else
			update(mid + 1, right, R, idx, val);
		// Updating while returning to parent nodes
		seg[node] = merge(seg[L], seg[R]);
	}

	template <typename Pred>
	int countPred(int left, int right, int node, int leftQuery, int rightQuery, ll x, Pred pred)
	{
		if (right < leftQuery || left > rightQuery)
			return 0;
		if (left >= leftQuery && right <= rightQuery)
			return pred(seg[node].range.begin(), seg[node].range.end(), x);
		return countPred(left, mid, L, leftQuery, rightQuery, x, pred) +
			   countPred(mid + 1, right, R, leftQuery, rightQuery, x, pred);
	}

public:
	MergeSortSegmentTree(const vector<ll> &arr)
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
	int countLess(int leftQuery, int rightQuery, ll x)
	{
		return countPred(0, size - 1, 0, leftQuery, rightQuery, x, [](auto begin, auto end, ll x)
						 { return std::lower_bound(begin, end, x) - begin; });
	}
	int countLessEq(int leftQuery, int rightQuery, ll x)
	{
		return countPred(0, size - 1, 0, leftQuery, rightQuery, x, [](auto begin, auto end, ll x)
						 { return std::upper_bound(begin, end, x) - begin; });
	}
	int countGreater(int leftQuery, int rightQuery, ll x)
	{
		return countPred(0, size - 1, 0, leftQuery, rightQuery, x, [](auto begin, auto end, ll x)
						 { return end - std::upper_bound(begin, end, x); });
	}
	int countGreaterEq(int leftQuery, int rightQuery, ll x)
	{
		return countPred(0, size - 1, 0, leftQuery, rightQuery, x, [](auto begin, auto end, ll x)
						 { return end - std::lower_bound(begin, end, x); });
	}
#undef L
#undef R
#undef mid
};

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
	// #ifndef ONLINE_JUDGE
	// 	freopen("input.txt", "r", stdin);
	// 	freopen("Output.txt", "w", stdout);
	// #endif //! ONLINE_JUDGE
	int t = 1;
	ll N, Q;
	// cin >> t;
	while (t--)
	{
		cin >> N >> Q;
		vector<ll> last(N);
		map<ll, int> lastPos;
		for (int i{}; i < N; i++)
		{
			ll x;
			cin >> x;
			if (lastPos.count(x) == 0)
				last[i] = -1;
			else
				last[i] = lastPos[x];
			lastPos[x] = i;
		}
		MergeSortSegmentTree segTree(last);
		for (int i{}; i < Q; i++)
		{
			ll L, R;
			cin >> L >> R;
			cout << segTree.countLess(L, R - 1, L) << endl;
		}
	}
	return 0;
}
