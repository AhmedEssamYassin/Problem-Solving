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
	int N;
	// cin >> t;
	while (t--)
	{
		string str;
		bool first = true;
		while (cin >> str)
		{
			reverse(str.begin(), str.end());
			if (not first)
				cout << " ";
			cout << str;
			first = false;
		}
	}
	return 0;
}