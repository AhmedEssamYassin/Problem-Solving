#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

bool canMake(ll x, ll N)
{
	// x * (x + 1) / 2 <= N
	return x * (x + 1) / 2 <= N;
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
		cin >> N;
		// x * (x + 1) / 2 <= N
		ll L{}, R = (1LL << 31), ans{};
		while (L <= R)
		{
			ll mid = ((L + R) >> 1);
			if (canMake(mid, N))
				ans = mid, L = mid + 1;
			else
				R = mid - 1;
		}
		ans += (N > ans * (ans + 1) / 2);
		cout << ans << endl;
	}
	return 0;
}