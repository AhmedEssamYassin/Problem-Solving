#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct PersistentSegmentTree
{
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		ll value;
		int L;
		int R;
		bitset<1001> shelf;
		Node() { value = 0, L = 0, R = 0; }
		Node(const ll &x)
		{
			value = x;
			L = 0;
			R = 0;
		}
		void merge(const Node &leftNode, const Node &rightNode)
		{
			value = (leftNode.value + rightNode.value);
		}
	};

	vector<Node> tree;
	int maxN;
	vector<int> versions;
	bitset<1001> mask;

	int build(const vector<ll> &arr, int left, int right)
	{
		int newNode = tree.size();
		tree.push_back(Node());

		if (left == right)
		{
			tree[newNode] = arr[left];
			return newNode;
		}

		tree[newNode].L = build(arr, left, mid);
		tree[newNode].R = build(arr, mid + 1, right);
		tree[newNode].merge(tree[tree[newNode].L], tree[tree[newNode].R]);
		return newNode;
	}

	int insert(int node, int left, int right, int i, int slot, int op)
	{
		Node copy = tree[node];
		int newNode = tree.size();
		tree.push_back(copy);

		if (left == right)
		{
			if (op == 1)
				tree[newNode].shelf.set(slot);
			else if (op == 2)
				tree[newNode].shelf.reset(slot);
			else if (op == 3)
				tree[newNode].shelf ^= mask;

			tree[newNode].value = tree[newNode].shelf.count();
			return newNode;
		}

		if (i <= mid)
			tree[newNode].L = insert(copy.L, left, mid, i, slot, op);
		else
			tree[newNode].R = insert(copy.R, mid + 1, right, i, slot, op);

		tree[newNode].merge(tree[tree[newNode].L], tree[tree[newNode].R]);
		return newNode;
	}

public:
	PersistentSegmentTree(int n, int m, int maxInsertions) : maxN(n)
	{
		for (int i{}; i < m; i++)
			mask[i] = 1;
		int capacity = (n + maxInsertions) * (__lg(max(1, n)) + 2) + 2;
		tree.reserve(capacity);
		tree.push_back(Node());
		versions.push_back(0);
	}

	// Constructs tree from base array. Returns new version index.
	int build(const vector<ll> &arr)
	{
		tree.reserve(tree.capacity() + 4 * arr.size());
		int root = build(arr, 0, maxN - 1);
		versions.push_back(root);
		return versions.size() - 1;
	}

	int insert(int version, int i, int slot, int op)
	{
		assert(version >= 0 && version < versions.size());
		int newRoot = insert(versions[version], 0, maxN - 1, i, slot, op);
		versions.push_back(newRoot);
		return versions.size() - 1;
	}

	ll getTotalBooks() { return tree[versions.back()].value; }

	// Duplicates a specific version exactly. Returns new version index.
	// Call when a time-step occurs but no actual data changes.
	int copyAndAppend(int k)
	{
		assert(k >= 0 && k < versions.size());
		versions.push_back(versions[k]);
		return versions.size() - 1;
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
		int n, m, q;
		cin >> n >> m >> q;
		PersistentSegmentTree pst(n, m, q);
		for (int ver{1}; ver <= q; ver++)
		{
			ll type, i, j, k;
			cin >> type;
			if (type == 1)
			{
				cin >> i >> j;
				--i, --j;
				pst.insert(ver - 1, i, j, 1);
			}
			else if (type == 2)
			{
				cin >> i >> j;
				--i, --j;
				pst.insert(ver - 1, i, j, 2);
			}
			else if (type == 3)
			{
				cin >> i;
				--i;
				pst.insert(ver - 1, i, -1, 3);
			}
			else
			{
				cin >> k;
				pst.copyAndAppend(k);
			}
			cout << pst.getTotalBooks() << endl;
		}
	}
	return 0;
}