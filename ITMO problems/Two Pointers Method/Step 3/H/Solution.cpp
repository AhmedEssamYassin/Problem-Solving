#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

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
		ll n, m, s, A, B;
		cin >> n >> m >> s >> A >> B;
		vector<ll> a(n), b(m);
		for (int i{}; i < n; i++)
			cin >> a[i];
		for (int i{}; i < m; i++)
			cin >> b[i];

		sort(a.rbegin(), a.rend());
		sort(b.rbegin(), b.rend());

		vector<ll> prefB(m + 1, 0);
		for (int i{}; i < m; i++)
			prefB[i + 1] = prefB[i] + b[i];

		ll curA = 0, best = 0;
		for (int cntA{}; cntA <= n; cntA++)
		{
			if (cntA > 0)
				curA += a[cntA - 1];
			ll remW = s - cntA * A;
			if (remW < 0)
				break;
			ll cntB = min((ll)m, remW / B);
			best = max(best, curA + prefB[cntB]);
		}
		cout << best;
	}
	return 0;
}
