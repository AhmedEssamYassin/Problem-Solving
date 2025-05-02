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
	ll N, M;
	cin >> t;
	while (t--)
	{
		cin >> N;
		vector<ll> P(N), pref(N, 0);
		map<ll, ll> vis;
		for (int i{}; i < N; i++)
			cin >> P[i];
		for (int i{}; i < N; i++)
		{
			if (!vis[P[i]] || P[i] != P[i - 1])
				pref[i] = (i > 0 ? pref[i - 1] : 0) + 1;
			else
				pref[i] = pref[i - 1];
			vis[P[i]]++;
		}
		cin >> M;
		while (M--)
		{
			ll L, R;
			cin >> L >> R;
			L--, R--;
			if (pref[R] == pref[L])
				cout << "-1 -1\n";
			else
			{
				cout << L + 1 << " ";
				ll idx = lower_bound(pref.begin(), pref.end(), pref[L] + 1) - pref.begin() + 1;
				cout << idx << endl;
			}
		}
	}
	return 0;
}