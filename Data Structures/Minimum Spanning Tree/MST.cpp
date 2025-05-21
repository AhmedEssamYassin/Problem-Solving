#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

/*
Spanning tree
Suppose we have a connected directed graph G = {V, E}
The spanning tree is any sub-Graph = {V', E'}, where V' = V and E' = |V| - 1
Number of spanning trees = (|E| Choose (|V|-1)) - number of cycles

1. A minimum spanning tree of a graph is unique, if the weight of all the edges are distinct.
   Otherwise, there may be multiple minimum spanning trees.
   (Specific algorithms typically output one of the possible minimum spanning trees).
2. Minimum spanning tree is also the tree with minimum product of weights of edges.
   (It can be easily proved by replacing the weights of all edges with their logarithms)
3. In a minimum spanning tree of a graph, the maximum weight of an edge is the minimum possible from all possible spanning trees of that graph.
   (This follows from the validity of Kruskal's algorithm).
   The maximum spanning tree (spanning tree with the sum of weights of edges being maximum) of a graph
   can be obtained similarly to that of the minimum spanning tree,
   by changing the signs of the weights of all the edges to their opposite and then applying any of the minimum spanning tree algorithm.
*/

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

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
    int t = 1;
    ll N, M, A;
    // cin >> t;
    while (t--)
    {
        /*
        N: the number of villages.
        M: the number of possible pipelines that can be built.
        A: the cost of building a water treatment plant in a village.
        */
        cin >> N >> M >> A;
        vector<Edge> graph;
        DSU disjointSet(N + 1);
        for (int i{}; i < M; i++)
        {
            ll u, v, c;
            cin >> u >> v >> c;
            graph.push_back({u, v, c});
        }
        sort(graph.begin(), graph.end()); // Sort edges by cost

        // Kruskal's MST logic
        ll mstCost = 0, totalComponents = N;

        for (const auto &[u, v, cost] : graph)
        {
            if (cost < A && !disjointSet.isSameComponent(u, v))
            {
                disjointSet.Union(u, v);
                mstCost += cost;
                totalComponents--;
            }
        }

        // Total cost is MST cost + cost of building plants in remaining components
        ll totalCost = mstCost + totalComponents * A;

        cout << totalCost << " " << totalComponents << endl;
    }
    return 0;
}