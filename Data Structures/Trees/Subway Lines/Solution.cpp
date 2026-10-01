#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct TreeAncestor
{
    int n, LOG;
    vector<int> up;                   // up[v * LOG + j] = 2^j-th ancestor of v, the root is its own parent
    vector<int> depth, in, out, tour; // Subtree of v is tour[in[v]..out[v]]

    TreeAncestor(const vector<vector<int>> &adj, int root)
        : n(adj.size()), LOG(__lg(n) + 1), up((size_t)n * LOG, root), depth(n), in(n), out(n), tour(n)
    {
        int timer = 0;
        vector<pair<int, int>> st{{root, 0}}; // Iterative DFS: node, next edge
        tour[in[root] = timer++] = root;
        while (!st.empty())
        {
            auto &[u, i] = st.back();
            if (i == (int)adj[u].size())
            {
                out[u] = timer - 1;
                st.pop_back();
                continue;
            }
            int v = adj[u][i++];
            if (v == par(u))
                continue;
            up[v * LOG] = u, depth[v] = depth[u] + 1;
            tour[in[v] = timer++] = v;
            st.push_back({v, 0});
        }
        for (int i = 1; i < n; i++) // Tour order: ancestors are filled before descendants
            for (int v = tour[i], j = 1; j < LOG; j++)
                up[v * LOG + j] = up[up[v * LOG + j - 1] * LOG + j - 1];
    }

    int par(int v, int j = 0) const { return up[v * LOG + j]; }

    int getDepth(int u) const { return depth[u]; }
    int subtreeSize(int u) const { return out[u] - in[u] + 1; }
    bool isAncestor(int u, int v) const { return in[u] <= in[v] && in[v] <= out[u]; } // u is an ancestor of v, or u == v

    int getKthAncestor(int v, int k) const // k = 0 is v itself, -1 if above the root
    {
        if (k < 0 || k > depth[v])
            return -1;
        for (int j = 0; k; j++, k >>= 1)
            if (k & 1)
                v = par(v, j);
        return v;
    }

    int getLCA(int u, int v) const
    {
        if (depth[u] < depth[v])
            swap(u, v);
        u = getKthAncestor(u, depth[u] - depth[v]);
        if (u == v)
            return u;
        for (int j = LOG - 1; j >= 0; j--)
            if (par(u, j) != par(v, j))
                u = par(u, j), v = par(v, j);
        return par(u);
    }

    int getDistance(int u, int v) const { return depth[u] + depth[v] - 2 * depth[getLCA(u, v)]; }

    bool onPath(int x, int u, int v) const // x is on the path u -> v
    {
        return (isAncestor(x, u) || isAncestor(x, v)) && isAncestor(getLCA(u, v), x);
    }

    int childAncestor(int u, int v) const // Child of u on the path to v, u must be an ancestor of v
    {
        return u == v ? u : getKthAncestor(v, depth[v] - depth[u] - 1);
    }

    int getKthNodeOnPath(int u, int v, int k) const // k = 0 is u, -1 if k is past v
    {
        int a = getLCA(u, v), d1 = depth[u] - depth[a], d2 = depth[v] - depth[a];
        if (k < 0 || k > d1 + d2)
            return -1;
        return k <= d1 ? getKthAncestor(u, k) : getKthAncestor(v, d1 + d2 - k);
    }

    int getCommonNode(int a, int b, int c) const // Meeting point of the paths between a, b, c
    {
        return getLCA(a, b) ^ getLCA(b, c) ^ getLCA(c, a); // Two of the three are equal
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
    // cin >> t;
    while (t--)
    {
        ll N, Q;
        cin >> N >> Q;
        vector<vector<int>> Tree(N + 1);
        int anyNode = 1;
        for (int i{}; i < N - 1; i++)
        {
            int u, v;
            cin >> u >> v;
            anyNode = u;
            Tree[u].push_back(v);
            Tree[v].push_back(u);
        }
        int root = 1;
        // If the tree is not rooted
        root = anyNode;
        TreeAncestor treeAnc(Tree, root);
        while (Q--)
        {
            int a, b, c, d;
            cin >> a >> b >> c >> d;
            // path(a, b) = path(a, LCA) -> path(LCA, b)
            ll sumOnPath = treeAnc.getDistance(a, b) + treeAnc.getDistance(c, d);
            ll common = min(treeAnc.getDistance(a, c) + treeAnc.getDistance(b, d), treeAnc.getDistance(a, d) + treeAnc.getDistance(b, c));
            if (sumOnPath < common)
                cout << 0 << endl;
            else
                cout << (sumOnPath - common) / 2 + 1 << endl;
        }
    }
    return 0;
}