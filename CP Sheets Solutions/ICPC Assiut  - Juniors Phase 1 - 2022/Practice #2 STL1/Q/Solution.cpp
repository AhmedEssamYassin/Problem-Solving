#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

const ll mod = 1e9 + 7;
ll mult64(const ll &a, const ll &b)
{
	return __int128_t(a) * b % mod;
}

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
	cin >> t;
	while (t--)
	{
		ll x;
		string str, cut;
		cin >> x >> str;
		// Perform the process until the string length is `x`
		/*
		The cut and paste actually is like separating the string into two parts:
		str[0...l - 1] and str[l...end]
		Since characters of the string are either '1', '2' or '3'
		The length of the string will increase by (str[l - 1] - '1') *  (len - l)
		*/
		ll l{};
		while (str.length() < x)
		{
			// Perform move action once
			l++;
			if (str[l - 1] == '1')
				continue;
			// Perform cut action once
			cut = str.substr(l);
			str.resize(l); // Truncate in place, equivalent to: str = str.substr(0, l);
			// Perform paste str[l - 1] times
			for (int i{}; i < str[l - 1] - '0'; i++)
				str += cut;
		}
		// Simulate the remaining process
		ll len = str.length();
		while (l < x)
		{
			// Perform move action once
			l++;
			// Perform cut action once
			// Perform paste str[l - 1] times
			len = (len % mod + mult64((str[l - 1] - '1'), (len - l + mod))) % mod;
		}
		cout << len << endl;
	}
	return 0;
}