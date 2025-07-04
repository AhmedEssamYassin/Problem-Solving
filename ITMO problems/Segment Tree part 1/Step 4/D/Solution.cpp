#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

const int maxValue = 41;
struct SegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		ll countDifferent{};
		bitset<maxValue> existing;
		// Constructors
		Node() {}
		Node(const ll &other) : countDifferent(other) {}
	};
	int size;
	vector<Node> seg;
	Node merge(const Node &leftNode, const Node &rightNode)
	{
		Node res;
		res.existing = (leftNode.existing | rightNode.existing);
		res.countDifferent = res.existing.count();
		return res;
	}
	void build(int left, int right, int node, const vector<ll> &arr)
	{
		if (left == right)
		{
			if (left < arr.size())
				seg[node].countDifferent = 1, seg[node].existing[arr[left]] = 1;
			return;
		}

		// Building left node
		build(left, mid, L, arr);

		// Building right node
		build(mid + 1, right, R, arr);

		// Returning to parent nodes
		seg[node] = merge(seg[L], seg[R]);
	}
	void update(int left, int right, int node, int idx, const ll &other)
	{
		if (left == right)
		{
			seg[node].countDifferent = 1;
			seg[node].existing.reset();
			seg[node].existing[other] = 1;
			return;
		}
		if (idx <= mid)
			update(left, mid, L, idx, other);
		else
			update(mid + 1, right, R, idx, other);

		// Updating while returning to parent nodes
		seg[node] = merge(seg[L], seg[R]);
	}
	Node query(int left, int right, int node, int leftQuery, int rightQuery)
	{
		// Out of range
		if (right < leftQuery || left > rightQuery)
			return Node(0); // A neutral value that doesn't affect the query
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
		seg = vector<Node>(2 * size, Node(0));
		build(0, size - 1, 0, arr);
	}
	void update(int idx, const ll &val)
	{
		update(0, size - 1, 0, idx, val);
	}
	ll query(int left, int right)
	{
		Node ans = query(0, size - 1, 0, left, right);
		return ans.countDifferent;
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
	int N, Q, type, Xi, Yi;
	cin >> N >> Q;
	vector<ll> vc(N);
	for (int i{}; i < N; i++)
		cin >> vc[i];
	SegmentTree segTree(vc);
	while (Q--)
	{
		cin >> type >> Xi >> Yi;
		if (type == 1)
		{
			Xi--, Yi--; // To be 0-based
			cout << segTree.query(Xi, Yi) << endl;
		}
		else
		{
			Xi--; // To be 0-based
			segTree.update(Xi, Yi);
		}
	}
	return 0;
}