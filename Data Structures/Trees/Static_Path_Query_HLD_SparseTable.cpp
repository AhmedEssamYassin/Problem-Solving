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

// Can be used for Idempotent functions: GCD, LCM, Maximum, Minimum, OR, AND and also XORBasis
ll bitCeil(const ll &N)
{
    ll res{1};
    while (res < N)
        res <<= 1;
    return res;
}

template <typename T>
struct SparseTable
{
    int size, LOG;
    vector<vector<T>> m;
    T merge(const T &a, const T &b)
    {
        return min(a, b);
    }
    void build(const vector<ll> &arr)
    {
        int N = arr.size();
        for (int i{}; i < N; i++)
            m[i][0] = arr[i];
        for (int k = 1; k < LOG; k++)
        {
            for (int i{}; i + (1 << k) - 1 < N; i++)
                m[i][k] = merge(m[i][k - 1], m[i + (1 << (k - 1))][k - 1]);
        }
    }
    SparseTable() {}
    SparseTable(const vector<ll> &arr)
    {
        int n = arr.size();
        LOG = (ll)(log2l(bitCeil(n)) + 1) + 1;
        m.resize(n, vector<T>(LOG, 0));
        build(arr);
    }

    T query(int L, int R) // 0-based
    {
        int len = R - L + 1;
        int k = 31 - __builtin_clz(len);
        return merge(m[L][k], m[R - (1 << k) + 1][k]);
    }
};

struct HeavyLightDecomposition
{
private:
    int timer = 0;
    vector<int> parent, depth, heavy, head, in, out, size;
    SparseTable<ll> SPT;

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
        SPT = SparseTable<ll>(baseArray);
    }

    ll getDepth(ll u) const
    {
        return depth[u];
    }

    ll queryPath(int u, int v)
    {
        ll res = LLONG_MAX; // Neutral value for the query
        while (head[u] != head[v])
        {
            if (depth[head[u]] < depth[head[v]])
                swap(u, v);
            res = SPT.merge(res, SPT.query(in[head[u]], in[u]));
            u = parent[head[u]];
        }
        if (depth[u] > depth[v])
            swap(u, v);
        res = SPT.merge(res, SPT.query(in[u], in[v]));
        return res;
    }

    ll querySubtree(int u)
    {
        return SPT.query(in[u], out[u]);
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