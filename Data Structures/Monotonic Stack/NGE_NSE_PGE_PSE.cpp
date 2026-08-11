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
    ll n;
    // cin >> t;
    while (t--)
    {
        cin >> n;
        vector<int> lcp;

        stack<int> st;

        // Elements can repeat but indices can NEVER, so instead of storing elements, store indices!
        vector<int> NGE(n, -1), NSE(n, -1), PGE(n, -1), PSE(n, -1); // Next/Previous Greater/Smaller Elements

        // Indices of NSE[], PSE[] represent indices of lcp[] and their values are the index of their NSE, PSE
        // Default value of all indices is -1 , so that if NO greater element found, they will be -1

        // 1. Compute Next Smaller Element (NSE)
        for (int i = 0; i < n; i++)
        {
            // ASYMMETRY RULE: Use '<=' (non-strict) on the "Next" side.
            // This ensures if duplicate minimums exist, the first one stops here,
            // preventing us from double-counting identical subarrays.
            while (!st.empty() && lcp[i] <= lcp[st.top()])
            {
                NSE[st.top()] = i;
                st.pop();
            }
            st.push(i);
        }

        // Clear the stack
        while (!st.empty())
            st.pop();

        // 2. Compute Previous Smaller Element (PSE)
        for (int i = n - 1; i >= 0; i--)
        {
            // ASYMMETRY RULE: Use '<' (strict) on the "Previous" side.
            // Combined with '<=' above, this guarantees each duplicate takes
            // exclusive "ownership" of a unique set of subarrays.
            while (!st.empty() && lcp[i] < lcp[st.top()])
            {
                PSE[st.top()] = i;
                st.pop();
            }
            st.push(i);
        }

        // Clear the stack
        while (!st.empty())
            st.pop();

        // 3. Compute Next Greater Element (NGE)
        for (int i = 0; i < n; i++)
        {
            // ASYMMETRY RULE: Use '>=' (non-strict) to handle duplicate maximums
            while (!st.empty() && lcp[i] >= lcp[st.top()])
            {
                NGE[st.top()] = i;
                st.pop();
            }
            st.push(i);
        }

        // Clear the stack
        while (!st.empty())
            st.pop();

        // 4. Compute Previous Greater Element (PGE)
        for (int i = n - 1; i >= 0; i--)
        {
            // ASYMMETRY RULE: Use '>' (strict) on the opposite side
            while (!st.empty() && lcp[i] > lcp[st.top()])
            {
                PGE[st.top()] = i;
                st.pop();
            }
            st.push(i);
        }
    }
    return 0;
}