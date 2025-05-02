#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
	int t = 1;
	ll N, Q;
	// cin >> t;
	while (t--)
	{
		string str;
		cin >> str;
		stack<char> stk;
		ll cnt{};
		for (int i{}; i < str.length(); i++)
		{
			if (str[i] == '(')
				stk.push(str[i]);
			else
			{
				if (not stk.empty())
					stk.pop(), cnt++;
			}
		}
		cout << 2 * cnt;
	}
	return 0;
}