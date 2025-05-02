#include <bits/stdc++.h>
using namespace std;
#define ll long long int
// #define endl "\n"

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
	int t = 1;
	ll N, K;
	// cin >> t;
	while (t--)
	{
		auto Ask = [&](ll left, ll right)
		{
			if (right - left + 1 == 1)
				return -1LL;
			cout << "? " << left << " " << right;
			cout << endl;
			ll x;
			cin >> x;
			return x;
		};

		cin >> N;
		// The second maximum will be so iff the first maximum exists in queried range
		// Otherwise, it would be the maximum
		ll L = 1, R = N;
		ll Smax = Ask(L, R);
		ll ans = -1;
		if (Ask(Smax, R) == Smax)
		{
			L = Smax + 1;
			ans = N;
			while (L <= R)
			{
				ll mid = ((L + R) >> 1);
				if (Ask(Smax, mid) == Smax)
				{
					ans = mid;
					R = mid - 1;
				}
				else
					L = mid + 1;
			}
		}
		else
		{
			R = Smax - 1;
			ans = 1;
			while (L <= R)
			{
				ll mid = ((L + R) >> 1);
				if (Ask(mid, Smax) == Smax)
				{
					ans = mid;
					L = mid + 1;
				}
				else
					R = mid - 1;
			}
		}
		cout << "! " << ans << endl;
	}
	return 0;
}