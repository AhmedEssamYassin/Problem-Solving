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
	ll N;
	// cin >> t;
	while (t--)
	{
		cin >> N;
		ll cnt = 1; // x = 1
		for (ll p2 = 2; p2 <= N; p2 <<= 1)
		{
			for (ll p3 = p2; p3 <= N;)
			{
				cnt++;
				if (p3 > N / 3)
					break;
				p3 *= 3;
			}
		}
		cout << cnt;
	}
	return 0;
}