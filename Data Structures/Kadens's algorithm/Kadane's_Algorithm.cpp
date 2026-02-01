#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

// Kadane's Algorithm to Maximum Sum Sub_array
// At each element: Either start a new contiguous sub_array or continue the previous sum
tuple<int, int, int> maxSubarraySum(const vector<ll> &vc)
{
    int maxSubSum = INT_MIN, currSum = 0, L = 0, R = 0, temp = 0;
    for (int i{}; i < vc.size(); i++)
    {
        currSum += vc[i];
        if (currSum > maxSubSum)
        {
            maxSubSum = currSum;
            L = temp + 1;
            R = i + 1;
        }
        if (currSum < 0)
        {
            currSum = 0;
            temp = i + 1;
        }
    }
    return {maxSubSum, L, R};
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifdef LOCAL
    freopen("input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif

    int t = 1, N;
    cin >> t;
    while (t--)
    {
        cin >> N;
        vector<ll> vc(N);
        for (int i{}; i < N; i++)
            cin >> vc[i];

        tuple<int, int, int> ans = maxSubarraySum(vc); //{maxSum, L, R}

        cout << get<0>(ans) << " " << get<1>(ans) << " " << get<2>(ans) << endl;
    }
    return 0;
}