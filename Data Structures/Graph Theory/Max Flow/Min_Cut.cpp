#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"
#define INF LLONG_MAX

class MaxFlow
{
public:
    int N;
    vector<vector<ll>> adj;
    vector<vector<ll>> capacity;
    vector<vector<ll>> paths;
    vector<deque<ll>> flowEdges;
    vector<bool> visited;
    MaxFlow(int _N)
    {
        N = _N;
        adj.assign(N + 1, vector<ll>());
        flowEdges.assign(N + 1, deque<ll>());
        visited.assign(N + 1, false);
        capacity.assign(N + 1, vector<ll>(N + 1, 0));
    }

    void addEdge(ll u, ll v, ll cap)
    {
        adj[u].push_back(v);
        adj[v].push_back(u); // Reverse edge for residual graph
        // To handle multiple edges make it +=
        // Capacity of 1 in the graph and residual graph means that it is Unweighted
        capacity[u][v] = cap;
        capacity[v][u] = cap; // if the graph is Undirected
    }

    ll BFS(int source, int sink, vector<ll> &parents)
    {
        fill(parents.begin(), parents.end(), -1);
        parents[source] = -2;
        queue<pair<ll, ll>> que;
        que.push({source, INF});

        while (!que.empty())
        {
            auto &[current, flow] = que.front();
            que.pop();

            for (const auto &next : adj[current])
            {
                if (parents[next] == -1 && capacity[current][next] > 0)
                {
                    parents[next] = current;
                    ll currentFlow = min(flow, capacity[current][next]);
                    if (next == sink) // If we reach the sink, return the flow
                        return currentFlow;
                    que.push({next, currentFlow});
                }
            }
        }
        return 0; // No augmenting path found
    }

    ll Edmond_Karp(int source, int sink)
    {
        ll flow = 0;

        vector<ll> parents(N + 1);

        ll augmentingFlow = 0;
        while ((augmentingFlow = BFS(source, sink, parents)))
        {
            flow += augmentingFlow;
            ll current = sink;
            while (current != source)
            {
                ll previous = parents[current];
                capacity[previous][current] -= augmentingFlow;
                capacity[current][previous] += augmentingFlow;
                current = previous;
            }
        }
        return flow;
    }
    void dfs(vector<ll> &path, int node, int sink)
    {
        path.push_back(node); // Add the current node to the path
        if (node == sink)
        {
            paths.push_back(path);
            return;
        }
        dfs(path, flowEdges[node].front(), sink);
        flowEdges[node].pop_front();
    }
    void extractPaths(vector<array<ll, 2>> &orgEdges, int src, int sink)
    {
        for (const auto &[u, v] : orgEdges)
            capacity[u][v]--;
        for (int u = 1; u <= sink; u++)
        {
            for (int v = 1; v <= sink; v++)
            {
                if (capacity[u][v] < 0)
                    flowEdges[u].push_back(v); // adding the edge
            }
        }
        while (!flowEdges[src].empty())
        {
            vector<ll> path;
            dfs(path, src, sink);
        }
    }
    // Find reachable nodes using DFS on the residual graph
    void minCutDFS(vector<bool> &visited, ll node)
    {
        visited[node] = true;
        for (ll next : adj[node])
        {
            if (!visited[next] && capacity[node][next] > 0)
                minCutDFS(visited, next); // Unvisited and residual capacity > 0
        }
    }

    // Find reachable nodes using BFS on the residual graph
    vector<bool> findReachable(int N, int src)
    {
        vector<bool> visited(N + 1, false);
        queue<int> q;
        q.push(src);
        visited[src] = true;

        while (!q.empty())
        {
            int cur = q.front();
            q.pop();

            for (auto &next : adj[cur])
            {
                if (!visited[next] && capacity[cur][next] > 0)
                {
                    visited[next] = true;
                    q.push(next);
                }
            }
        }
        return visited;
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
    ll N, M;
    // cin >> t;
    while (t--)
    {
        cin >> N >> M;
        ll src = 1, sink = N;
        MaxFlow maxflow(sink + 1);
        while (M--)
        {
            ll u, v;
            cin >> u >> v;
            // Graph is directed and Unweighted
            // Capacity of 1 in the graph and residual graph means that it is Unweighted
            maxflow.addEdge(u, v, 1);
        }
        cout << maxflow.Edmond_Karp(src, sink) << endl;
        vector<bool> reachable = maxflow.findReachable(N, src);
        // maxflow.dfs(reachable, 1);
        for (int u = 1; u <= N; u++)
        {
            for (auto &v : maxflow.adj[u])
            {
                if (reachable[u] && !reachable[v])
                    // u is reachable but v is not
                    cout << u << " " << v << endl;
            }
        }
    }
    return 0;
}