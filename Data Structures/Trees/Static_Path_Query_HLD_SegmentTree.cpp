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
    ll sum;
    // Constructors
    Node() {}
    Node(ll val) : sum(val) {}
};

struct SegmentTree
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
    Node query(int left, int right, int node, int leftQuery, int rightQuery)
    {
        // Out of range
        if (right < leftQuery || left > rightQuery)
            return 0; // A value that doesn't affect the query

        // The whole range is the answer
        if (left >= leftQuery && right <= rightQuery)
            return seg[node];

        // ONLY a part of this segment belongs to the query
        Node leftSegment = query(left, mid, L, leftQuery, rightQuery);
        Node rightSegment = query(mid + 1, right, R, leftQuery, rightQuery);
        return merge(leftSegment, rightSegment);
    }

public:
    SegmentTree() {}
    SegmentTree(const vector<ll> &arr)
    {
        size = 1;
        int n = arr.size();
        while (size < n)
            size <<= 1;
        seg = vector<Node>(2 * size, 0);
        build(0, size - 1, 0, arr);
    }

    Node merge(const Node &leftNode, const Node &rightNode)
    {
        Node res;
        res.sum = (leftNode.sum + rightNode.sum);
        return res;
    }

    void update(int idx, const ll &val)
    {
        update(0, size - 1, 0, idx, val);
    }
    ll query(int left, int right)
    {
        Node ans = query(0, size - 1, 0, left, right);
        return ans.sum;
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
    SegmentTree segTree;

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
        segTree = SegmentTree(baseArray);
    }

    ll getDepth(ll u) const
    {
        return depth[u];
    }

    ll queryPath(int u, int v)
    {
        Node res = 0; // Neutral value for the query
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
        return res.sum;
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

        cin >> Q;
        while (Q--)
        {
        }
    }
    return 0;
}