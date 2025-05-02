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
	string str;
	cin >> t;
	while (t--)
	{
		cin >> str;
		int freq[4] = {0};
		// 123
		// 112233
		int L{}, R{}, len{}, ans{INT_MAX};
		while (R < str.length())
		{
			freq[str[R] - '0']++;
			len++;
			bool cond;
			while (freq[str[L] - '0'] - 1 > 0)
				freq[str[L++] - '0']--, len--;
			if (freq[1] * freq[2] * freq[3] > 0)
				ans = min(ans, len);
			R++;
		}
		if (freq[1] * freq[2] * freq[3] > 0)
			ans = min(ans, len);
		if (ans == INT_MAX)
			cout << 0 << endl;
		else
			cout << ans << endl;
	}
	return 0;
}