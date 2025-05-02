#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

bool isOk(ll v, ll K, ll N)
{
	ll cnt = v;
	while (v)
		v /= K, cnt += v;
	return cnt >= N;
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
	ll N, K;
	// cin >> t;
	while (t--)
	{
		cin >> N >> K;
		// What minimum allowable value `v` can take to let him write >= N lines of code before he falls asleep.
		ll L{}, R = (1LL << 40), ans{};
		while (L <= R)
		{
			ll mid = ((L + R) >> 1);
			if (isOk(mid, K, N))
				ans = mid, R = mid - 1;
			else
				L = mid + 1;
		}
		cout << ans;
	}
	return 0;
}