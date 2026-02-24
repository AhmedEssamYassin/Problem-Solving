#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

class CompressedRange
{
private:
	vector<ll> init;
	void compress(vector<ll> &vec)
	{
		sort(vec.begin(), vec.end());
		vec.erase(unique(vec.begin(), vec.end()), vec.end());
	}

public:
	CompressedRange(vector<ll> &vec)
	{
		init = vec;
		compress(init);
	}
	int index(ll val)
	{
		return lower_bound(init.begin(), init.end(), val) - init.begin();
	}
	ll initVal(int idx)
	{
		return init[idx];
	}
};

struct PersistentSegmentTree
{
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		ll value;
		int L;
		int R;
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

	int insert(int node, int left, int right, int i, ll val)
	{
		Node copy = tree[node];
		int newNode = tree.size();
		tree.push_back(copy);

		if (left == right)
		{
			tree[newNode].value += val;
			return newNode;
		}

		if (i <= mid)
			tree[newNode].L = insert(copy.L, left, mid, i, val);
		else
			tree[newNode].R = insert(copy.R, mid + 1, right, i, val);

		tree[newNode].merge(tree[tree[newNode].L], tree[tree[newNode].R]);
		return newNode;
	}

	Node query(int node, int left, int right, int leftQuery, int rightQuery)
	{
		if (!node || leftQuery > right || rightQuery < left)
			return Node();
		if (leftQuery <= left && right <= rightQuery)
			return tree[node];

		Node leftSegment = query(tree[node].L, left, mid, leftQuery, rightQuery);
		Node rightSegment = query(tree[node].R, mid + 1, right, leftQuery, rightQuery);
		Node res;
		res.merge(leftSegment, rightSegment);
		return res;
	}

	int getKth(int nodeL, int nodeR, ll left, ll right, int k)
	{
		if (left == right)
			return left;

		int countLeft = tree[tree[nodeR].L].value - tree[tree[nodeL].L].value;

		if (countLeft >= k)
			return getKth(tree[nodeL].L, tree[nodeR].L, left, mid, k);
		return getKth(tree[nodeL].R, tree[nodeR].R, mid + 1, right, k - countLeft);
	}

public:
	PersistentSegmentTree(int n, int maxInsertions) : maxN(n)
	{
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

	// Inserts `value` at `idx` based on a previous version. Returns new version index.
	// Call to create a new historical state
	int insert(int version, int i, ll x)
	{
		assert(version >= 0 && version < versions.size());
		int newRoot = insert(versions[version], 0, maxN - 1, i, x);
		versions.push_back(newRoot);
		return versions.size() - 1;
	}

	// Updates a version IN-PLACE. Does NOT create a new version.
	// Call when modifying a state without needing to keep the old one.
	void update(int version, int i, ll x)
	{
		assert(version >= 1 && version < versions.size());
		// Overwrite the root of the existing version instead of appending a new one
		versions[version] = insert(versions[version], 0, maxN - 1, i, x);
	}

	// Queries sum/result in [leftQuery, rightQuery] for a specific version.
	// Call to answer standard range queries on historical data.
	ll query(int version, int leftQuery, int rightQuery)
	{
		assert(version >= 0 && version < versions.size());
		return query(versions[version], 0, maxN - 1, leftQuery, rightQuery).value;
	}

	// Finds the K-th smallest element in [versionL, versionR]
	// Call for that type of problems where tree stores value frequencies.
	int getKth(int versionL, int versionR, int k)
	{
		assert(versionL > 0 && versionL < versions.size());
		assert(versionR >= 0 && versionR < versions.size());
		return getKth(versions[versionL - 1], versions[versionR], 0, maxN - 1, k);
	}

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
		int N, Q;
		cin >> N >> Q;
		vector<ll> vc(N);
		for (int i{}; i < N; i++)
			cin >> vc[i];
		CompressedRange comp(vc);
		for (int i{}; i < N; i++)
			vc[i] = comp.index(vc[i]);
		PersistentSegmentTree pst(N, Q);
		for (int i{}; i < N; i++)
			pst.insert(i, vc[i], 1);

		while (Q--)
		{
			int L, R, k;
			cin >> L >> R >> k;
			int pos = pst.getKth(L, R, k);
			cout << comp.initVal(pos) << endl;
		}
	}
	return 0;
}