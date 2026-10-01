#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

// Max flow on a tree between two nodes is simply the minimum edge capacity along the unique path from source to sink.

// Path query: f over the edge weights of a path. f must be associative and commutative.
// id is the neutral value, f(x, id) = x, and is the answer for an empty path (u == v). Write it as T: 0LL, not 0.
// f = min -> LLONG_MAX, max -> LLONG_MIN (or 0LL if weights >= 0), sum / gcd / xor / or -> 0LL, and -> -1LL
template <typename T, typename F>
struct TreeAncestor
{
    int n, LOG;
    F f;
    T id;
    vector<int> up;                    // up[v * LOG + j] = 2^j-th ancestor of v, the root is its own parent
    vector<T> agg;                    // agg[v * LOG + j] = f over the weights of the 2^j edges above v
    vector<int> depth, in, out, tour; // Subtree of v is tour[in[v]..out[v]]
 
    template <typename G>
    TreeAncestor(const G &adj, int root, F f, T id)
        : n(adj.size()), LOG(__lg(n) + 1), f(f), id(id), up((size_t)n * LOG, root), agg((size_t)n * LOG, id), depth(n), in(n), out(n), tour(n)
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
            int v = adj[u][i].first;
            auto w = adj[u][i++].second;
            if (v == par(u))
                continue;
            up[v * LOG] = u, agg[v * LOG] = w, depth[v] = depth[u] + 1;
            tour[in[v] = timer++] = v;
            st.push_back({v, 0});
        }
        for (int i = 1; i < n; i++) // Tour order: ancestors are filled before descendants
            for (int v = tour[i], j = 1; j < LOG; j++)
            {
                int mid = par(v, j - 1);
                up[v * LOG + j] = par(mid, j - 1);
                agg[v * LOG + j] = f(agg[v * LOG + j - 1], agg[mid * LOG + j - 1]);
            }
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
 
    T queryUp(int v, int k) const // f over the k edges above v, 0 <= k <= depth[v], id if k = 0
    {
        T res = id;
        for (int j = 0; k; j++, k >>= 1)
            if (k & 1)
                res = f(res, agg[v * LOG + j]), v = par(v, j);
        return res;
    }
 
    T queryPath(int u, int v) const // f over the edge weights on the path u -> v, id if u == v
    {
        int a = getLCA(u, v);
        return f(queryUp(u, depth[u] - depth[a]), queryUp(v, depth[v] - depth[a]));
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
    ll N, M, Q;
    // cin >> t;
    while (t--)
    {
        cin >> N >> M;
        // The given graph is connected and has no cycles, self-loops, or multi-edges.
        // That means it's a Tree!
        vector<vector<pair<int, ll>>> Tree(N + 1);
        int anyNode = 1;
        for (int i{}; i < N - 1; i++)
        {
            ll u, v, w;
            cin >> u >> v >> w;
            anyNode = u;
            Tree[u].emplace_back(v, w);
            Tree[v].emplace_back(u, w);
        }
        int root = 1;
        // If the tree is not rooted
        root = anyNode;
        TreeAncestor treeAnc(Tree, root, [](ll a, ll b) { return min(a, b); }, LLONG_MAX);
        cin >> Q;
        while (Q--)
        {
            ll src, sink;
            cin >> src >> sink;
            // path(src, sink) = path(src, LCA) -> path(LCA, sink)
            cout << treeAnc.queryPath(src, sink) << endl;
        }
    }
    return 0;
}
