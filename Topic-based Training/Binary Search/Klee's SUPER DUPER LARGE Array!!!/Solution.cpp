#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

ll sum(ll start, ll end, ll n)
{
	return (n * (start + end) / 2);
}

bool isOk(ll N, ll mid, ll K)
{
	// K K+1 K+2 ... K+N-1
	// 1 .. mid
	// mid+1 .. N
	return (sum(K, K + mid - 1, mid) > sum(K + mid, K + N - 1, N - mid));
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
	ll N, x, y, k;
	cin >> t;
	while (t--)
	{
		cin >> N >> k;
		ll L{1}, R = N;
		ll i;
		while (L <= R)
		{
			ll mid = (L + R) / 2;
			if (isOk(N, mid, k))
				i = mid, R = mid - 1;
			else
				L = mid + 1;
		}
		ll ans1 = abs(sum(k, k + i - 1, i) - sum(k + i, k + N - 1, N - i));
		i--;
		ll ans2 = abs(sum(k, k + i - 1, i) - sum(k + i, k + N - 1, N - i));
		cout << min(ans1, ans2) << endl;
	}
	return 0;
}