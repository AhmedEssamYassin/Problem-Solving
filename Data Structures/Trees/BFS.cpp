#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

/*
Unlike Graphs, a Tree doesn't need a `visited` array for BFS because it has NO cycles.
In BFS on a Tree, we simply avoid revisiting the parent node (previous node).
*/
void BFS(vector<vector<ll>> &Tree, int root)
{
    queue<int> que;
    vector<int> parent(Tree.size(), -1);

    que.push(root);
    while (!que.empty())
    {
        int u = que.front();
        que.pop();

        for (const auto &v : Tree[u])
        {
            if (v != parent[u])
            {
                parent[v] = u;
                que.push(v);
            }
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
        BFS(Tree, root);
    }
    return 0;
}
