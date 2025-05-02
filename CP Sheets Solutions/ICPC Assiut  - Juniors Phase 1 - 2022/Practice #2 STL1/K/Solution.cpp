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
	// cin >> t;
	while (t--)
	{
		cin >> N;
		stack<ll> allStack, maxStack;
		while (N--)
		{
			ll query, x;
			cin >> query;
			if (query == 1)
			{
				cin >> x;
				allStack.push(x);
				maxStack.push(max(!maxStack.empty() ? maxStack.top() : (ll)-1e18 - 1, x));
				cout << maxStack.top() << endl;
			}
			else
			{
				allStack.pop();
				maxStack.pop();
				cout << maxStack.top() << endl;
			}
		}
	}
	return 0;
}