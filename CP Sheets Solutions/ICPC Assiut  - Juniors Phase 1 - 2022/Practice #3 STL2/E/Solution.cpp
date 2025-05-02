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
		string str;
		cin >> str;
		map<string, ll> mp; // {string, digit}
		for (int i{}; i < 10; i++)
		{
			string encryptedDigit;
			cin >> encryptedDigit;
			mp[encryptedDigit] = i;
		}
		for (int i{}; i < str.length(); i += 10)
			cout << mp[str.substr(i, 10)];
	}
	return 0;
}