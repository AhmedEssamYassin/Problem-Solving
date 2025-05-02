#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

bool isOk(const vector<ll> &a, ll K, long double val)
{
	long double rem{};
	for (int i{}; i < a.size(); i++)
	{
		if (a[i] > val)
			rem += (a[i] - val) * (1 - K / 100.0);
		else if (a[i] < val)
		{
			if (rem >= (val - a[i]))
				rem -= (val - a[i]);
			else
				return false;
		}
	}
	return true;
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
		vector<ll> a(N);
		for (int i{}; i < N; i++)
			cin >> a[i];
		sort(a.begin(), a.end(), greater<>());
		long double L{}, R = (1000), ans{};
		while (R - L > 1e-7)
		{
			long double mid = (L + R) / 2;
			if (isOk(a, K, mid))
				ans = mid, L = mid;
			else
				R = mid;
		}
		cout << fixed << setprecision(7) << ans << endl;
	}
	return 0;
}