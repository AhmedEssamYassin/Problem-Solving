#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

/*
A heavy child of a node is the child with the largest subtree size rooted at the child.
A light child of a node is any child that is not a heavy child.
A heavy edge connects a node to its heavy child.
A light edge connects a node to any of its light children.
A heavy path is the path formed by a collection heavy edges.
A light path is the path formed by a collection light edges.
*/

struct Node
{
	vector<ll> range;
	// Constructors
	Node() {}
	Node(const ll &value) { range.push_back(value); }
};
struct MergeSortSegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	int size;
	vector<Node> seg;

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
	MergeSortSegmentTree() {}
	MergeSortSegmentTree(const vector<ll> &arr)
	{
		size = 1;
		int n = arr.size();
		while (size < n)
			size <<= 1;
		seg = vector<Node>(2 * size, Node());
		build(0, size - 1, 0, arr);
	}

	Node merge(const Node &leftNode, const Node &rightNode)
	{
		Node res;
		res.range.resize(leftNode.range.size() + rightNode.range.size());
		std::merge(leftNode.range.begin(), leftNode.range.end(), rightNode.range.begin(), rightNode.range.end(), res.range.begin());
		return res;
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

struct HeavyLightDecomposition
{
private:
	int timer = 0;
	vector<int> parent, depth, heavy, head, in, out, size;
	MergeSortSegmentTree segTree;

	int dfsSize(const vector<vector<ll>> &Tree, int u)
	{
		size[u] = 1;
		int maxSubtree = 0;
		for (const auto &v : Tree[u])
		{
			if (v == parent[u])
				continue;
			parent[v] = u;
			depth[v] = depth[u] + 1;
			size[u] += dfsSize(Tree, v);
			if (size[v] > maxSubtree)
			{
				heavy[u] = v;
				maxSubtree = size[v];
			}
		}
		return size[u];
	}

	void dfsHld(const vector<vector<ll>> &Tree, const vector<ll> &values, vector<ll> &baseArray, int u, int h)
	{
		head[u] = h;
		in[u] = timer++;
		baseArray[in[u]] = values[u];
		if (heavy[u] != -1)
			dfsHld(Tree, values, baseArray, heavy[u], h);
		for (const auto &v : Tree[u])
		{
			if (v != parent[u] && v != heavy[u])
				dfsHld(Tree, values, baseArray, v, v);
		}
		out[u] = timer - 1;
	}

public:
	HeavyLightDecomposition(const vector<vector<ll>> &Tree, int root, const vector<ll> &values)
	{
		int N = Tree.size();
		parent.assign(N, -1);
		depth.resize(N);
		heavy.assign(N, -1);
		head.resize(N);
		in.resize(N);
		out.resize(N);
		size.resize(N);

		dfsSize(Tree, root);
		vector<ll> baseArray(N);
		dfsHld(Tree, values, baseArray, root, 0);
		segTree = MergeSortSegmentTree(baseArray);
	}

	ll getDepth(ll u) const
	{
		return depth[u];
	}

	ll queryPath(int u, int v, int k)
	{
		ll res = 0; // Neutral value for the query
		while (head[u] != head[v])
		{
			if (depth[head[u]] < depth[head[v]])
				swap(u, v);
			res += segTree.countGreater(in[head[u]], in[u], k);
			u = parent[head[u]];
		}
		if (depth[u] > depth[v])
			swap(u, v);
		res += segTree.countGreater(in[u], in[v], k);
		return res;
	}

	ll querySubtree(int u, int k)
	{
		return segTree.countGreater(in[u], out[u], k);
	}
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
	ll N, Q;
	cin >> t;
	while (t--)
	{
		cin >> N >> Q;
		vector<vector<ll>> Tree(N + 1);
		vector<ll> val(N + 1);
		for (int u = 1; u <= N; u++)
			cin >> val[u];
		int anyNode = 1;
		for (int i{}; i < N - 1; i++)
		{
			ll u, v;
			cin >> u >> v;
			anyNode = u;
			Tree[u].push_back(v);
			Tree[v].push_back(u);
		}

		ll root = 1;
		// If the tree is not rooted
		root = anyNode;
		HeavyLightDecomposition hld(Tree, root, val);
		while (Q--)
		{
			int u, v, k;
			cin >> u >> v >> k;
			cout << hld.queryPath(u, v, k) << endl;
		}
	}
	return 0;
}