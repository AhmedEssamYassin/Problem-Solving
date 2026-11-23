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
	cin >> t;
	while (t--)
	{
		int n;
		string s;
		cin >> n >> s;
		int bad = 0;
		for (int i{}; i < n / 2; i++)
		{
			if (s[i] != s[n - 1 - i])
				bad++;
		}
		string ans(n + 1, '0');
		for (int x = bad; x <= n - bad; x++)
		{
			if ((n & 1) || ((x - bad) % 2 == 0))
				ans[x] = '1';
		}
		cout << ans << endl;
	}
	return 0;
}
