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
	int t = 1;
	ll N, K;
	cin >> t;
	while (t--)
	{
		cin >> N >> K;
		vector<ll> a(N), h(N);
		for (int i{}; i < N; i++)
			cin >> a[i];
		for (int i{}; i < N; i++)
			cin >> h[i];
		ll L{}, R = 0, ans = 0, sum{};
		while (R < N)
		{
			/*
			3 2 4 1 8
			4 4 2 4 1
			*/
			sum += a[R++];
			if (sum > K && L <= R)
				sum -= a[L++];
			if (sum <= K) // Valid
				ans = max(ans, R - L);
			while (R < N && h[R - 1] % h[R] == 0)
			{
				sum += a[R++];
				while (sum > K && L <= R)
					sum -= a[L++];
				if (sum <= K) // Valid
					ans = max(ans, R - L);
			}
			L = R;
			sum = 0;
		}
		cout << ans << endl;
	}
	return 0;
}