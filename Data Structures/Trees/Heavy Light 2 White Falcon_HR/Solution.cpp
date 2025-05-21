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

const ll mod = 1e9 + 7;
ll mult64(const ll &a, const ll &b)
{
	return (__int128_t(a) * b) % mod;
}
ll modPow(ll N, ll power, ll mod)
{
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
ll add64(const ll &a, const ll &b)
{
	ll res = a + b;
	if (res < 0)
		res += mod;
	if (res >= mod)
		res -= mod; // Faster than doing %= everytime
	return res;
}
ll modDiv(const ll &a, const ll &b)
{
	return mult64(a, modPow(b, mod - 2, mod));
}

struct Node
{
	ll value;
	Node() {}
	Node(const ll &N) : value(N) {}
};

struct LazySegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	struct LazyNode
	{
		ll cnt, inc;
		LazyNode() {}
		LazyNode(ll a, ll d) : cnt(a), inc(d) {}
	};
	ll sum(const ll &N)
	{
		return mult64(N, mult64((N + 1), 500000004)); // 500000004 is 2 mod inverse (1e9 + 7)
	}
	int size;
	vector<Node> seg;
	vector<LazyNode> lazy;

	void build(int left, int right, int node, const vector<ll> &arr)
	{
		// If the segment has only one element, leaf node
		if (left == right)
		{
			if (left < arr.size())
				seg[node] = arr[left];
			return;
		}
		// Recursively build the left child
		build(left, mid, L, arr);
		// Recursively build the right child
		build(mid + 1, right, R, arr);
		// Merge the children values
		seg[node] = merge(seg[L], seg[R]);
	}
	void push(int left, int right, int node)
	{
		// Propagate the value
		if (lazy[node].cnt == 0)
			return;
		ll len = right - left + 1;
		seg[node] = add64(seg[node].value, mult64(lazy[node].cnt, len));
		seg[node] = add64(seg[node].value, mult64(lazy[node].inc, sum(len - 1)));
		// If the node is not a leaf
		if (left != right)
		{
			// Update the lazy values for the left child
			lazy[L].cnt = add64(lazy[L].cnt, lazy[node].cnt);
			lazy[L].inc = add64(lazy[L].inc, lazy[node].inc);
			// Update the lazy values for the right child
			lazy[R].cnt = add64(lazy[R].cnt, add64(lazy[node].cnt, mult64((mid - left + 1), lazy[node].inc)));
			lazy[R].inc = add64(lazy[R].inc, lazy[node].inc);
		}
		// Reset the lazy value
		lazy[node] = LazyNode(0, 0);
	}
	void update(int left, int right, int node, int leftQuery, int rightQuery, const ll &a, const ll &d)
	{
		push(left, right, node);
		// If the range is invalid, return
		if (left > rightQuery || right < leftQuery)
			return;
		// If the range matches the segment
		if (left >= leftQuery && right <= rightQuery)
		{
			// Update the lazy value
			lazy[node].cnt = add64(a, mult64((left - leftQuery), d));
			lazy[node].inc = d;
			// Apply the update immediately
			push(left, right, node);
			return;
		}
		// Recursively update the left child
		update(left, mid, L, leftQuery, rightQuery, a, d);
		// Recursively update the right child
		update(mid + 1, right, R, leftQuery, rightQuery, a, d);
		// Merge the children values
		seg[node] = merge(seg[L], seg[R]);
	}
	Node query(int left, int right, int node, int leftQuery, int rightQuery)
	{
		// Apply the pending updates if any
		push(left, right, node);
		// If the range is invalid, return a value that does NOT to affect other queries
		if (left > rightQuery || right < leftQuery)
			return 0;

		// If the range matches the segment
		if (left >= leftQuery && right <= rightQuery)
			return seg[node];

		return merge(query(left, mid, L, leftQuery, rightQuery), query(mid + 1, right, R, leftQuery, rightQuery));
	}

public:
	LazySegmentTree(const vector<ll> &arr = vector<ll>())
	{
		size = 1;
		int n = arr.size();
		while (size < n)
			size <<= 1;
		seg = vector<Node>(2 * size, 0);
		lazy = vector<LazyNode>(2 * size, LazyNode(0, 0));
		build(0, size - 1, 0, arr);
	}
	Node merge(const Node &leftNode, const Node &rightNode)
	{
		Node res = add64(leftNode.value, rightNode.value);
		return res;
	}
	void update(int left, int right, const ll &a, const ll &d)
	{
		update(0, size - 1, 0, left, right, a, d);
	}
	ll query(int left, int right)
	{
		Node ans = query(0, size - 1, 0, left, right);
		return ans.value;
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
	LazySegmentTree segTree;

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
		segTree = LazySegmentTree(baseArray);
	}

	ll getDepth(ll u) const
	{
		return depth[u];
	}

	void updateNode(int u, ll x)
	{
		segTree.update(in[u], in[u], x, x);
	}

	void updateSubtree(int u, ll x)
	{
		segTree.update(in[u], out[u], x, x);
	}

	void updatePath(int u, int v, ll x)
	{
		vector<pair<int, int>> vc1, vc2;

		// Collect path segments (unchanged)
		while (head[u] != head[v])
		{
			if (depth[head[u]] > depth[head[v]])
			{
				vc1.emplace_back(in[head[u]], in[u]);
				u = parent[head[u]];
			}
			else
			{
				vc2.emplace_back(in[head[v]], in[v]);
				v = parent[head[v]];
			}
		}

		// Now both are on the same head
		if (depth[u] > depth[v])
			vc1.emplace_back(in[v], in[u]);
		else
			vc2.emplace_back(in[u], in[v]);

		// Reverse vc2 to get correct order from LCA to v
		reverse(vc2.begin(), vc2.end());

		// Process all segments from u to LCA (vc1)
		ll offset = 1; // Starting with x
		for (const auto &[l, r] : vc1)
		{
			/*
			For each segment in `u` to `LCA` path, values must DECREASE as we go from l to r
			because we're going from depth to root in the segment, but we want
			values to INCREASE as we go from u to v
			*/

			// The highest offset in this segment
			offset += (r - l);

			// Update with decreasing progression (high to low)
			segTree.update(l, r, mult64(offset, x), -x);

			offset++;
		}

		// Process all segments from LCA to v (vc2)
		for (const auto &[l, r] : vc2)
		{
			/*
			For `LCA` to `v` path, values naturally INCREASE as we go from l to r,
			which aligns with our desired progression
			*/
			segTree.update(l, r, mult64(offset, x), x);
			offset += (r - l + 1);
		}
	}

	ll queryPath(int u, int v)
	{
		Node res = 0;
		while (head[u] != head[v])
		{
			if (depth[head[u]] < depth[head[v]])
				swap(u, v);
			res = segTree.merge(res, segTree.query(in[head[u]], in[u]));
			u = parent[head[u]];
		}
		if (depth[u] > depth[v])
			swap(u, v);
		res = segTree.merge(res, segTree.query(in[u], in[v]));
		return res.value;
	}

	ll querySubtree(int u)
	{
		return segTree.query(in[u], out[u]);
	}
};

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);

	int t = 1;
	ll N, Q;
	// cin >> t;
	while (t--)
	{
		cin >> N >> Q;
		vector<vector<ll>> Tree(N + 1);
		ll anyNode = 1;
		for (int i{}; i < N - 1; i++)
		{
			ll u, v;
			cin >> u >> v;
			u++, v++;

			anyNode = u;
			Tree[u].push_back(v);
			Tree[v].push_back(u);
		}
		ll root = anyNode;
		HeavyLightDecomposition hld(Tree, root, vector<ll>(N + 1, 0));

		while (Q--)
		{
			ll type, u, v, x;
			cin >> type;
			if (type == 1)
			{
				cin >> u >> v >> x;
				hld.updatePath(++u, ++v, x);
			}
			else
			{
				cin >> u >> v;
				cout << hld.queryPath(++u, ++v) << endl;
			}
		}
	}
	return 0;
}
