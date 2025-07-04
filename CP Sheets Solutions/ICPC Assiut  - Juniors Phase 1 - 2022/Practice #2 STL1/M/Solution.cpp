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
		cin >> N; // Number of lines
		stack<ll> stk;
		stk.push(1);
		string command;
		bool flag = true;
		ll n;
		ll X = 0; // To track the value of X
		while (N--)
		{
			cin >> command;
			if (command == "add")
			{
				if (X + stk.top() > (1LL << 32) - 1) // if X overflows int32_t
				{
					flag = false;
					break;
				}
				X += stk.top();
			}
			else if (command == "for")
			{
				cin >> n;
				stk.push(min(stk.top() * n, (1LL << 32)));
			}
			else // end
				stk.pop();
		}
		if (X > (1LL << 32) - 1 || flag == false)
			cout << "OVERFLOW!!!";
		else
			cout << X;
	}
	return 0;
}