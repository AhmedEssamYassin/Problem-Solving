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
		ll n, s;
		cin >> n >> s;
		vector<ll> vc(n);
		for (int i{}; i < n; i++)
			cin >> vc[i];

		ll curSum = 0, ans = 0;
		int L = 0;
		for (int R{}; R < n; R++)
		{
			curSum += vc[R];
			while (curSum > s)
			{
				curSum -= vc[L];
				L++;
			}
			ll len = R - L + 1;
			ans += len * (len + 1) / 2;
		}
		cout << ans;
	}
	return 0;
}
