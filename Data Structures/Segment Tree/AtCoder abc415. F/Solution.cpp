#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct SegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		ll prefLen, suffLen, maxLen;
		char firstChar, lastChar;
		int len;
		// Constructors
		Node() : prefLen(0), suffLen(0), maxLen(0), firstChar('#'), lastChar('#'), len(0) {}
		Node(char c) : prefLen(1), suffLen(1), maxLen(1), firstChar(c), lastChar(c), len(1) {}
	};
	int size;
	vector<Node> seg;
	Node merge(const Node &leftNode, const Node &rightNode)
	{
		Node res;
		res.len = leftNode.len + rightNode.len;
		res.firstChar = leftNode.firstChar;
		res.lastChar = rightNode.lastChar;

		res.prefLen = (leftNode.prefLen == leftNode.len && leftNode.lastChar == rightNode.firstChar)
						  ? leftNode.len + rightNode.prefLen
						  : leftNode.prefLen;
		res.suffLen = (rightNode.suffLen == rightNode.len && rightNode.firstChar == leftNode.lastChar)
						  ? rightNode.len + leftNode.suffLen
						  : rightNode.suffLen;

		res.maxLen = max({leftNode.maxLen, rightNode.maxLen,
						  (leftNode.lastChar == rightNode.firstChar ? leftNode.suffLen + rightNode.prefLen : 0)});
		return res;
	}
	void build(int left, int right, int node, const string &str)
	{
		if (left == right) // Leaf Node (single element)
		{
			if (left < str.size()) // Making sure we are inside the boundaries of the array
				seg[node] = str[left];
			return;
		}
		// Building left node
		build(left, mid, L, str);

		// Building right node
		build(mid + 1, right, R, str);

		// Returning to parent nodes
		seg[node] = merge(seg[L], seg[R]);
	}
	void update(int left, int right, int node, int idx, const char &val)
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
	Node query(int left, int right, int node, int leftQuery, int rightQuery)
	{
		// Out of range
		if (right < leftQuery || left > rightQuery)
			return Node(); // A value that doesn't affect the query

		// The whole range is the answer
		if (left >= leftQuery && right <= rightQuery)
			return seg[node];

		// ONLY a part of this segment belongs to the query
		Node leftSegment = query(left, mid, L, leftQuery, rightQuery);
		Node rightSegment = query(mid + 1, right, R, leftQuery, rightQuery);
		return merge(leftSegment, rightSegment);
	}

public:
	SegmentTree(const string &str)
	{
		size = 1;
		int n = str.size();
		while (size < n)
			size <<= 1;
		seg = vector<Node>(2 * size);
		build(0, size - 1, 0, str);
	}
	void update(int idx, const char &val)
	{
		update(0, size - 1, 0, idx, val);
	}
	ll query(int left, int right)
	{
		Node ans = query(0, size - 1, 0, left, right);
		return ans.maxLen;
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
	// cin >> t;
	while (t--)
	{
		string str;
		cin >> N >> Q >> str;
		SegmentTree segTree(str);
		while (Q--)
		{
			ll type, i, L, R;
			char x;
			cin >> type;
			if (type == 1)
			{
				cin >> i >> x;
				segTree.update(--i, x);
			}
			else
			{
				cin >> L >> R;
				cout << segTree.query(--L, --R) << endl;
			}
		}
	}
	return 0;
}