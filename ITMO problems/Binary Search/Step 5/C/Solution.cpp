#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

ll countLE(const vector<ll> &a, const vector<ll> &b, ll x)
{
	ll cnt = 0;
	int j = (int)b.size() - 1;
	for (int i{}; i < (int)a.size(); i++)
	{
		while (j >= 0 && a[i] + b[j] > x)
			j--;
		cnt += j + 1;
	}
	return cnt;
}

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
		ll n, k;
		cin >> n >> k;
		vector<ll> a(n), b(n);
		for (int i{}; i < n; i++)
			cin >> a[i];
		for (int i{}; i < n; i++)
			cin >> b[i];
		sort(a.begin(), a.end());
		sort(b.begin(), b.end());

		ll L = a[0] + b[0], R = a.back() + b.back(), ans = R;
		while (L <= R)
		{
			ll mid = L + (R - L) / 2;
			if (countLE(a, b, mid) >= k)
			{
				ans = mid;
				R = mid - 1;
			}
			else
				L = mid + 1;
		}
		cout << ans;
	}
	return 0;
}
