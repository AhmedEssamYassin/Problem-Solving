#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

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
		cin >> N;
		vector<ll> vc(N);
		for (int i{}; i < N; i++)
			cin >> vc[i];

		// Elements can repeat but indices can NEVER, so instead of storing elements, store indices!
		stack<int> stk;
		vector<int> NSE(N, N);	// Next Smaller Element, default to N (beyond the end)
		vector<int> PSE(N, -1); // Previous Smaller Element, default to -1 (before the beginning)
		// Values of NSE[] and PSE[] are the index of their NSE, PSE

		// Compute Next Smaller Element (NSE)
		for (int i = 0; i < N; i++)
		{
			while (!stk.empty() && vc[i] < vc[stk.top()])
			{
				NSE[stk.top()] = i; // Store the index of the next smaller element
				stk.pop();
			}
			stk.push(i);
		}

		// Clear the stack for the next computation
		while (!stk.empty())
			stk.pop();

		// Compute Previous Smaller Element (PSE)
		for (int i = N - 1; i >= 0; i--)
		{
			while (!stk.empty() && vc[i] < vc[stk.top()])
			{
				PSE[stk.top()] = i; // Store the index of the previous smaller element
				stk.pop();
			}
			stk.push(i);
		}

		ll maxRect = *max_element(vc.begin(), vc.end());
		for (int i{}; i < N; i++)
			maxRect = max(maxRect, vc[i] * (NSE[i] - PSE[i] - 1));
		cout << maxRect << endl;
	}
	return 0;
}