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
        ll value;
        LazyNode() {}
        LazyNode(const ll &N) : value(N) {}
        LazyNode operator+=(const LazyNode &RHS)
        {
            value += RHS.value;
            return *this;
        }
    };
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
        if (lazy[node].value == 0)
            return;
        seg[node].value += lazy[node].value;
        // If the node is not a leaf
        if (left != right)
        {
            // Update the lazy values for the left child
            lazy[L] += lazy[node];
            // Update the lazy values for the right child
            lazy[R] += lazy[node];
        }
        // Reset the lazy value
        lazy[node] = 0;
    }
    void update(int left, int right, int node, int leftQuery, int rightQuery, const ll &val)
    {
        push(left, right, node);
        // If the range is invalid, return
        if (left > rightQuery || right < leftQuery)
            return;
        // If the range matches the segment
        if (left >= leftQuery && right <= rightQuery)
        {
            // Update the lazy value
            lazy[node] = val;
            // Apply the update immediately
            push(left, right, node);
            return;
        }
        // Recursively update the left child
        update(left, mid, L, leftQuery, rightQuery, val);
        // Recursively update the right child
        update(mid + 1, right, R, leftQuery, rightQuery, val);
        // Merge the children values
        seg[node] = merge(seg[L], seg[R]);
    }
    Node query(int left, int right, int node, int leftQuery, int rightQuery)
    {
        // Apply the pending updates if any
        push(left, right, node);
        // If the range is invalid, return a value that does NOT to affect other queries
        if (left > rightQuery || right < leftQuery)
            return LLONG_MIN;

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
        seg = vector<Node>(2 * size, LLONG_MIN);
        lazy = vector<LazyNode>(2 * size, 0); // Initialize with a Not-possible value
        build(0, size - 1, 0, arr);
    }
    Node merge(const Node &leftNode, const Node &rightNode)
    {
        Node res = max(leftNode.value, rightNode.value);
        return res;
    }
    void update(int left, int right, const ll &val)
    {
        update(0, size - 1, 0, left, right, val);
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

    void updateNode(int u, ll val)
    {
        segTree.update(in[u], in[u], val);
    }

    void updateSubtree(int u, ll val)
    {
        segTree.update(in[u], out[u], val);
    }

    void updatePath(int u, int v, ll val)
    {
        while (head[u] != head[v])
        {
            if (depth[head[u]] < depth[head[v]])
                swap(u, v);
            segTree.update(in[head[u]], in[u], val);
            u = parent[head[u]];
        }
        if (depth[u] > depth[v])
            swap(u, v);
        segTree.update(in[u], in[v], val);
    }

    ll queryPath(int u, int v)
    {
        Node res = LLONG_MIN; // Neutral value for the query
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
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
    int t = 1;
    ll N, Q;
    // cin >> t;
    while (t--)
    {
        cin >> N;
        vector<vector<ll>> Tree(N + 1);
        int anyNode = -1;
        for (int i{}; i < N - 1; i++)
        {
            ll u, v;
            cin >> u >> v;
            if (!~anyNode)
                anyNode = u;
            Tree[u].push_back(v);
            Tree[v].push_back(u);
        }
        ll root = 1;
        // If the tree is not rooted
        // root = anyNode;
        HeavyLightDecomposition hld(Tree, root, vector<ll>(N + 1, 0));

        cin >> Q;
        while (Q--)
        {
            string type;
            ll t, x, a, b;
            cin >> type;
            if (type == "add")
            {
                cin >> t >> x;
                hld.updateSubtree(t, x);
            }
            else
            {
                cin >> a >> b;
                cout << hld.queryPath(a, b) << endl;
            }
        }
    }
    return 0;
}
