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

		map<char, char> closing{{'(', ')'}, {'[', ']'}, {'{', '}'}};
		stack<char> stk;
		bool valid = true; // Assuming it's valid until some violation occurs
		for (const char &C : str)
		{
			if (string("({[").find(C) != -1) // If it's an opening of some bracket
				stk.push(C);
			else
			{
				if (!stk.empty() && C == closing[stk.top()])
					stk.pop();
				else
					valid = false;
			}
		}
		if (valid && stk.empty()) // If all brackets were closed (found closing matches)
			cout << "yes";
		else
			cout << "no";
	}
	return 0;
}