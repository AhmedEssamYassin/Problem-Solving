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
	int N;
	// cin >> t;
	while (t--)
	{
		string str;
		cin >> str;
		stack<char> stk;
		for (const char &C : str)
		{
			if (!stk.empty() && stk.top() == C)
				stk.pop();
			else
				stk.push(C);
		}
		// Now the resulting string is in the stack
		// but the way we retrieve it (stk.top()) will make it reversed
		string ans("");
		while (!stk.empty())
			ans += stk.top(), stk.pop();
		reverse(ans.begin(), ans.end()); // Reverse it to retrieve the original order
		cout << ans;
	}
	return 0;
}