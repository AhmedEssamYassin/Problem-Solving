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
	ll N, len;
	// cin >> t;
	while (t--)
	{
		string str;
		cin >> str;
		N = str.length();
		// Map for bracket pairs
		map<char, char> closing = {{'(', ')'}, {'{', '}'}, {'[', ']'}, {'<', '>'}};
		map<char, char> opening = {{')', '('}, {'}', '{'}, {']', '['}, {'>', '<'}};

		vector<ll> d(N, -1);		// Position of corresponding open bracket, or -1 if it doesn't exist.
		vector<ll> c(N, -1);		// Position of earliest open bracket, such that substring s[c[j], j] is a regular bracket sequence.
		stack<pair<char, int>> stk; // Stores (bracket, index)
		for (int i{}; i < N; i++)
		{
			if (closing.count(str[i])) // '(', '{', '[' or '<'
				stk.push({str[i], i});
			else // ')', '}', ']' or '>'
			{
				if (stk.empty() || stk.top().first != opening[str[i]])
				{
					d[i] = c[i] = -1;
					while (!stk.empty())
						stk.pop(); // To start searching a valid substring
				}
				else
				{
					d[i] = stk.top().second;
					stk.pop();
					c[i] = d[i]; // At least

					// Checking if there exists an earlier open bracket forming a valid sequence
					if (d[i] - 1 >= 0 && c[d[i] - 1] != -1)
						c[i] = c[d[i] - 1];
				}
			}
		}
		ll maxLen{};
		for (int i{}; i < N; i++)
		{
			if (c[i] != -1 && i - c[i] + 1 > maxLen)
				maxLen = i - c[i] + 1;
		}

		int cnt{};
		for (int i{}; i < N; i++)
			cnt += (c[i] != -1 && i - c[i] + 1 == maxLen);
		cout << maxLen << " ";
		if (maxLen == 0)
			return cout << 1, 0;
		cout << cnt;
	}
	return 0;
}