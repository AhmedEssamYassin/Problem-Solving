#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

ll cntUnhappy(const vector<ll> &a, int N, long double avgWealth)
{
	ll cnt{};
	for (int i{}; i < N; i++)
		cnt += (a[i] < avgWealth);
	return cnt;
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
	cin >> t;
	while (t--)
	{
		cin >> N;
		vector<ll> a(N);
		ll sum{}, maxPos, mx = LLONG_MIN;
		for (int i{}; i < N; i++)
		{
			cin >> a[i];
			sum += a[i];
			if (a[i] > mx)
				maxPos = i, mx = a[i];
		}
		ll cnt{};
		// mx := mx + X
		// 31 / 6 / 2 = 5.1666667 / 2 =  2.5833335
		// mx = 25
		// (Sum + x) / (2 * N)
		ll L{}, R = (1LL << 60), ans = -1;
		while (L <= R)
		{
			ll mid((L + R) >> 1);
			a[maxPos] += mid;
			if (cntUnhappy(a, N, (sum + mid) / (2.0 * N)) > N / 2)
				ans = mid, R = mid - 1;
			else
				L = mid + 1;
			a[maxPos] -= mid;
		}
		cout << ans << endl;
	}
	return 0;
}