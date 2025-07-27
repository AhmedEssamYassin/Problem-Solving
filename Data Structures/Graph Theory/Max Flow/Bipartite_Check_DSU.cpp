#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct Edge
{
    ll u, v, cost;
    Edge(ll u, ll v, ll cost) : u(u), v(v), cost(cost) {}
    bool operator<(const Edge &e) const { return cost < e.cost; }
};

struct DSU
{
    vector<ll> parent, size; // Representative
    int N;
    // The leader should be the parent of itself
    DSU(int _N)
    {
        N = _N;
        parent.resize(N + 1);
        size.resize(N + 1, 1); // Each component if of size 1 initially
        iota(parent.begin(), parent.end(), 0);
    }
    ll Find(ll node)
    {
        if (parent[node] == node)
            return node;

        return parent[node] = Find(parent[node]); // Path compression
    }
    void Union(ll u, ll v)
    {
        ll rep1 = Find(u);
        ll rep2 = Find(v);
        if (rep1 == rep2)
            return;
        // Small-to-large technique
        if (size[rep1] > size[rep2])
            swap(rep1, rep2); // representative of smaller set comes first
        parent[rep1] = rep2;
        size[rep2] += size[rep1];
    }
    bool isSameComponent(ll u, ll v)
    {
        return (Find(u) == Find(v));
    }
    set<ll> findConnectedComponents()
    {
        set<ll> st;
        // Traverse all vertices
        for (int i = 1; i <= N; i++)
            st.insert(Find(parent[i]));

        return st;
    }
};

bool isBipartite(vector<vector<int>> &adj)
{
    int N = adj.size();
    DSU dsu(N + 1);
    for (int i = 0; i < N; i++)
    {
        for (int j = 1; j < adj[i].size(); j++)
            dsu.Union(adj[i][j], adj[i][j - 1]);
    }

    // Check for bipartite violations
    for (int i = 0; i < N; i++)
    {
        for (const int &j : adj[i])
        {
            if (dsu.isSameComponent(i, j))
                return false; // Not bipartite
        }
    }

    return true; // Bipartite
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
    int t = 1;
    ll N;
    cin >> t;
    while (t--)
    {
    }
    return 0;
}
