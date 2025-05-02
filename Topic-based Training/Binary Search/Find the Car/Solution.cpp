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
	ll N, d, k, q;
	cin >> t;
	while (t--)
	{
		cin >> N >> k >> q;
		vector<ll> a(k + 1), b(k + 1);
		a[0] = 0, b[0] = 0;
		for (int i{1}; i <= k; i++)
			cin >> a[i];
		for (int i{1}; i <= k; i++)
			cin >> b[i];

		while (q--)
		{
			cin >> d;
			int R = upper_bound(a.begin(), a.end(), d) - a.begin();
			R--;
			ll numerator = (d - a[R >= 0 ? R : 0]) * (b[R + 1 <= k ? R + 1 : k] - b[R >= 0 ? R : 0]);
			ll denominator = (a[R + 1 <= k ? R + 1 : k] - a[R >= 0 ? R : 0]);
			ll ans = b[R >= 0 ? R : 0] + numerator / (denominator ? denominator : 1);
			cout << ans << " ";
		}
		cout << endl;
	}
	return 0;
}