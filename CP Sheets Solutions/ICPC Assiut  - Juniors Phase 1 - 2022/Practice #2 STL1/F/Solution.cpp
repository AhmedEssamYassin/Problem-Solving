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
	string str;
	// cin >> t;
	while (cin >> str) // Number of test cases is not given
	{
		string ans(""), prefix("");
		bool beginning = false;
		for (const char &C : str)
		{
			if (C == '[')
			{
				beginning = true;
				ans = prefix + ans;
				prefix = "";
			}
			else if (C == ']')
			{
				beginning = false;
				ans = prefix + ans;
				prefix = "";
			}
			else
			{
				if (beginning)
					prefix += C;
				else
					ans += C;
			}
		}
		ans = prefix + ans;
		cout << ans << endl;
	}
	return 0;
}