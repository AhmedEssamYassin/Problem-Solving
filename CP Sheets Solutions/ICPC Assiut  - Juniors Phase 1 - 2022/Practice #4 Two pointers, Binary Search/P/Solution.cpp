#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

/*
Use this tests as a hint
6
4150492894710
012345678910
1023456789
301018563798482
01010102345678910
0000111112345678910
*/

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
	int curCase = 1;
	while (t--)
	{
		string str;
		cin >> str;
		N = str.length();
		map<ll, ll> freq;

		ll L = 0, R = 0;
		ll minLen = INT_MAX;
		ll bestLeft = -1, bestRight = -1;
		ll uniqueDigitsCount = 0;
		ll countTens{};
		while (R < N)
		{
			if (freq[str[R] - '0'] == 0)
				uniqueDigitsCount++;
			// We only count a 10 we we have a 1 and 0, (we count when we are at '0')
			if (R > 0 && str[R] - '0' == 0 && str[R - 1] - '0' == 1)
				countTens++;
			freq[str[R] - '0']++;
			// Relying on just frequency of '0' and '1' will not work in cases like 00...11...
			// We need to track 10s to maintain order
			while (uniqueDigitsCount == 10 && freq[0] >= 2 && freq[1] >= 2 && countTens > 0)
			{
				// Update minimum length substring
				if (R - L + 1 < minLen)
				{
					minLen = R - L + 1;
					bestLeft = L;
					bestRight = R;
				}
				freq[str[L] - '0']--;
				if (freq[str[L] - '0'] == 0)
					uniqueDigitsCount--;
				// Removing a 1 is enough to exclude a 10, (when removing 1 from 10 it becomes 0, so we exclude it when we are at 1)
				if (L + 1 < N && str[L] - '0' == 1 && str[L + 1] - '0' == 0)
					countTens--;
				L++;
			}
			R++;
		}

		cout << "Case " << curCase++ << ": ";
		if (minLen != INT_MAX)
			cout << "YES " << bestLeft + 1 << " " << bestRight + 1 << endl;
		else
			cout << "NO" << endl;
	}
	return 0;
}