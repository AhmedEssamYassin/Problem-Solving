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
		string str, ans;
		cin >> str;
		auto countOccurrence = [&](const string &substring)
		{
			ll len = substring.length();
			ll cnt{};
			for (int i{}; i < N - len + 1; i++)
				cnt += (str.substr(i, len) == substring);
			return cnt;
		};
		ll maxOcc{};
		for (int i{}; i < N - 1; i++)
		{
			string curSubString = str.substr(i, 2);
			ll cnt = countOccurrence(curSubString);
			if (cnt > maxOcc)
			{
				maxOcc = cnt;
				ans = curSubString;
			}
		}
		cout << ans;
	}
	return 0;
}