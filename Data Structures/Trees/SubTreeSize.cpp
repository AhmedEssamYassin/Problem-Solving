#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

/*
Unlike Graphs, a Tree doesn't need a `visited` array for DFS because it has NO cycles.
In DFS on a Tree, we simply avoid revisiting the parent node (previous node).
*/
vector<int> subtreeSize;
void DFS(vector<vector<ll>> &Tree, int u, int prev)
{
    subtreeSize[u] = 1;
    for (const auto &v : Tree[u])
    {
        if (v != prev)
        {
            DFS(Tree, v, u);
            subtreeSize[u] += subtreeSize[v];
        }
    }
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
        subtreeSize.assign(N + 1, 0);
        DFS(Tree, root, -1);
    }
    return 0;
}
