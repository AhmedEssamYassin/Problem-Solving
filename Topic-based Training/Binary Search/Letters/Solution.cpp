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
	ll N, m;
	// cin >> t;
	while (t--)
	{
		cin >> N >> m;
		vector<ll> a(N), b(m), pref(N, 0);
		for (int i{}; i < N; i++)
			cin >> a[i];
		pref[0] = a[0];
		for (int i{1}; i < N; i++)
			pref[i] = pref[i - 1] + a[i];
		for (int i{}; i < m; i++)
		{
			cin >> b[i];
			int idx = lower_bound(pref.begin(), pref.end(), b[i]) - pref.begin();
			cout << idx + 1 << " " << b[i] - (idx > 0 ? pref[idx - 1] : 0) << endl;
		}
	}
	return 0;
}