#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

class TreeAncestor
{
    vector<vector<ll>> up;
    vector<int> depth;
    ll LOG;

    vector<int> in, out, tourList, subtreeSize;
    int tourTime;

    void BFS(vector<vector<ll>> &Tree, int root, vector<int> &parent)
    {
        queue<ll> que;
        parent.assign(Tree.size(), -1);
        depth.assign(Tree.size(), 0);

        que.push(root);
        parent[root] = -1;
        depth[root] = 0;
        while (!que.empty())
        {
            int u = que.front();
            que.pop();
            for (const auto &v : Tree[u])
            {
                if (v != parent[u])
                {
                    parent[v] = u;
                    depth[v] = depth[u] + 1;
                    que.push(v);
                }
            }
        }
    }

    void dfs(vector<vector<ll>> &Tree, int u, int par, ll d)
    {
        in[u] = tourTime;
        tourList[tourTime] = u;
        subtreeSize[u] = 1;
        tourTime++;
        for (auto &v : Tree[u])
        {
            if (v != par)
            {
                dfs(Tree, v, u, d + 1);
                subtreeSize[u] += subtreeSize[v];
            }
        }
        out[u] = tourTime - 1;
    }

public:
    TreeAncestor(vector<vector<ll>> &Tree, int root, int N)
    {
        LOG = 0;
        while ((1LL << LOG) <= N)
            LOG++;

        vector<int> parent;
        BFS(Tree, root, parent);

        up = vector<vector<ll>>(N + 1, vector<ll>(LOG, -1));

        for (int v = 1; v <= N; v++)
            up[v][0] = parent[v];

        for (int j = 1; j < LOG; j++)
        {
            for (int v = 1; v <= N; v++)
            {
                if (up[v][j - 1] != -1)
                    up[v][j] = up[up[v][j - 1]][j - 1];
            }
        }

        // Initialize data for extended queries
        tourTime = 0;
        subtreeSize.assign(N + 1, 0);
        in.assign(N + 1, 0);
        out.assign(N + 1, 0);
        tourList.assign(N + 1, 0);
        dfs(Tree, root, -1, 0);
    }

    ll getDepth(ll u) const
    {
        return depth[u];
    }

    // Return k-th ancestor of `node` (0-based: k = 0 means a itself)
    ll getKthAncestor(ll node, ll k) const
    {
        for (ll j = LOG - 1; j >= 0; j--)
        {
            if (k >= (1LL << j))
            {
                node = up[node][j];
                if (node == -1)
                    return -1;
                k -= (1LL << j);
            }
        }
        return node;
    }

    ll getLCA(ll u, ll v) const
    {
        if (u == v)
            return u;

        if (depth[u] < depth[v])
            swap(u, v);

        for (ll j = LOG - 1; j >= 0; j--)
        {
            if (depth[u] - (1LL << j) >= depth[v])
                u = up[u][j];
        }

        if (u == v)
            return u;

        for (ll j = LOG - 1; j >= 0; j--)
        {
            if (up[u][j] != -1 && up[u][j] != up[v][j])
            {
                u = up[u][j];
                v = up[v][j];
            }
        }

        return up[u][0];
    }

    // Check if `u` is ancestor of `v`
    bool isAncestor(int u, int v) const
    {
        return (in[u] <= in[v]) && (in[v] <= out[u]);
    }

    // Check if x lies on the path from `u` to `v`
    bool onPath(int x, int u, int v) const
    {
        return (isAncestor(x, u) || isAncestor(x, v)) && isAncestor(getLCA(u, v), x);
    }

    // Return distance between nodes `u` and `v`
    ll getDistance(int u, int v) const
    {
        return depth[u] + depth[v] - 2 * depth[getLCA(u, v)];
    }

    // Return child of `u` that is on the path to `v`
    int childAncestor(int u, int v) const
    {
        if (u == v)
            return u;
        int x = v;
        for (int j = LOG - 1; j >= 0; j--)
        {
            if (up[x][j] != -1 && depth[up[x][j]] > depth[u])
                x = up[x][j];
        }
        return x;
    }

    // Return k-th node on the path from `u` to `v`
    int getKthNodeOnPath(int u, int v, int k) const
    {
        int anc = getLCA(u, v);
        int d1 = depth[u] - depth[anc];
        int d2 = depth[v] - depth[anc];
        if (k < 0 || k > d1 + d2)
            return -1;
        if (k <= d1)
            return getKthAncestor(u, k);
        return getKthAncestor(v, d1 + d2 - k);
    }

    // Return the common node among three nodes a, b, and c
    int getCommonNode(int a, int b, int c) const
    {
        int x = getLCA(a, b);
        int y = getLCA(b, c);
        int z = getLCA(c, a);
        return (x ^ y ^ z);
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
        // root = anyNode;
        TreeAncestor treeAnc(Tree, root, N);
        cin >> Q;
        while (Q--)
        {
            ll a, b, c;
            cin >> a >> b >> c;
            // path(a, b) = path(a, LCA) -> path(LCA, b)
            // dist(a, b) = depth[a] + depth[b] - 2 * depth[lca]
            ll lca = treeAnc.getLCA(a, b);
            if (c >= treeAnc.getDistance(a, b))
                cout << b << endl;
            else
            {
                if (c <= treeAnc.getDistance(a, lca))
                    cout << treeAnc.getKthAncestor(a, c) << endl;
                else
                {
                    ll rem = c - treeAnc.getDistance(a, lca);
                    ll steps_from_b = treeAnc.getDistance(b, lca) - rem;
                    cout << treeAnc.getKthAncestor(b, steps_from_b) << endl;
                }
            }
        }
    }
    return 0;
}
