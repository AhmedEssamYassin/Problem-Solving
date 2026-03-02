#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

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
        int N, M;
        cin >> N >> M;
        vector<vector<int>> G(N + 1);
        for (int i{}; i < M; i++)
        {
            int u, v;
            cin >> u >> v;
            G[u].push_back(v);
        }
        vector<bool> visited(N + 1, false);
        stack<int> topoSort;
        function<void(int)> dfs = [&](int u)
        {
            visited[u] = true;
            for (const int &v : G[u])
            {
                if (!visited[v])
                    dfs(v);
            }
            topoSort.push(u);
        };

        for (int i = 1; i <= N; i++)
        {
            if (!visited[i])
                dfs(i);
        }

        vector<int> dp(N + 1, 0);
        while (!topoSort.empty())
        {
            int u = topoSort.top();
            topoSort.pop();
            for (const int &v : G[u])
                dp[v] = max(dp[v], dp[u] + 1);
        }

        int maxLen = *max_element(dp.begin(), dp.end());
        cout << maxLen;
    }
    return 0;
}
