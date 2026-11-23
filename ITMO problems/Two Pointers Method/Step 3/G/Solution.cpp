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
	// cin >> t;
	while (t--)
	{
		ll n, c;
		string s;
		cin >> n >> c >> s;

		ll cntA = 0, cntB = 0, rudeness = 0, L = 0, best = 0;
		for (int R{}; R < n; R++)
		{
			if (s[R] == 'a')
				cntA++;
			else if (s[R] == 'b')
			{
				cntB++;
				rudeness += cntA;
			}
			while (rudeness > c)
			{
				if (s[L] == 'a')
				{
					cntA--;
					rudeness -= cntB;
				}
				else if (s[L] == 'b')
					cntB--;
				L++;
			}
			best = max(best, R - L + 1);
		}
		cout << best;
	}
	return 0;
}
