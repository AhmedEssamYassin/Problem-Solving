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
	int t, N, K;
	cin >> t;
	while (t--)
	{
		cin >> N >> K;
		ll ans;
		ll L{1}, R = 2e18 - 1;
		while (L <= R)
		{
			ll mid = L + (R - L) / 2;

			ll Val = mid - (mid / N); // Number of numbers that are NOT divisible by N
			if (Val == K)
			{
				ans = mid - (mid % N == 0);
				break;
			}
			else if (Val > K)
				R = mid - 1;
			else
				L = mid + 1;
		}
		cout << ans << endl;
	}
	return 0;
}