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
		ll sum;

		// Constructors
		Node() {}
		Node(ll val) : sum(val) {}
	};
	int size;
	vector<Node> seg;
	Node merge(const Node &leftNode, const Node &rightNode)
	{
		Node res;
		res.sum = leftNode.sum + rightNode.sum;
		return res;
	}
	void build(int left, int right, int node, const vector<ll> &arr)
	{
		if (left == right)
		{
			if (left < arr.size())
				seg[node] = Node(arr[left]);
			return;
		}
		// Building left node
		build(left, mid, L, arr);

		// Building right node
		build(mid + 1, right, R, arr);

		// Returning to parent nodes
		seg[node] = merge(seg[L], seg[R]);
	}
	void update(int left, int right, int node, int idx)
	{
		if (left == right)
		{
			seg[node].sum ^= 1;
			return;
		}
		if (idx <= mid)
			update(left, mid, L, idx);
		else
			update(mid + 1, right, R, idx);
		// Updating while returning to parent nodes
		seg[node] = merge(seg[L], seg[R]);
	}
	Node query(int left, int right, int node, int k)
	{
		if (left == right)
			return left;
		if (k <= seg[L].sum)
			return query(left, mid, L, k);
		else
			return query(mid + 1, right, R, k - seg[L].sum);
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
	void update(int idx)
	{
		update(0, size - 1, 0, idx);
	}
	ll query(int k)
	{
		Node ans = query(0, size - 1, 0, k);
		return ans.sum;
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
	ll N, M, i, k, q;
	cin >> N >> M;
	vector<ll> vc(N);
	for (int i{}; i < N; i++)
		cin >> vc[i];

	SegmentTree segTree(vc);
	while (M--)
	{
		cin >> q;
		if (q == 1)
		{
			cin >> i;
			segTree.update(i);
		}
		else
		{
			cin >> k;
			cout << segTree.query(k + 1) << endl;
		}
	}
	return 0;
}