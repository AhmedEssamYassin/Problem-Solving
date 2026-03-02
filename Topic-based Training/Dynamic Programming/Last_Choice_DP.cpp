#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

/*
DP Style: Multiple Choices with Last Choice Constraint

Problem Type:
- At each step (e.g., day/time/position), choose one among `k` options.
- Consecutive steps cannot repeat the same choice (or a specific subset of choices).
- Goal: maximize/minimize cumulative value across `n` steps.

DP Definition:
- dp[n][k]: Best score achievable at step n when the k-th option is chosen at this step.

Transition:
- dp[i][k] = max(dp[i - 1][j]) + cost[i][k] for all j ≠ k
- This ensures that the same choice is not picked in consecutive steps.

Base Case:
- dp[0][k] = initial cost of choosing k-th option at step 0

Final Answer:
- max(dp[n - 1][k]) over all k

Time Complexity: O(N * K)
*/

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
        int n;
        cin >> n;
        vector<int> vc(n);
        for (int i = 0; i < n; i++)
            cin >> vc[i];

        if (n <= 1)
            return cout << n, 0;

        // dp[i][0/1] = max zigzag length ending at i
        // 0 = last transition was decreasing (curr < prev)
        // 1 = last transition was increasing (curr > prev)
        vector<vector<int>> dp(n, vector<int>(2, 1));
        int maxLen = 1;
        for (int i = 1; i < n; i++)
        {
            for (int j = 0; j < i; j++)
            {
                if (vc[i] > vc[j])
                    dp[i][1] = max(dp[i][1], dp[j][0] + 1);
                else if (vc[i] < vc[j])
                    dp[i][0] = max(dp[i][0], dp[j][1] + 1);
            }
            maxLen = max(maxLen, max(dp[i][0], dp[i][1]));
        }

        cout << maxLen;
    }
    return 0;
}