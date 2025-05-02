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
	ll N, Q;
	// cin >> t;
	while (t--)
	{
		cin >> N;
		vector<ll> aliens(N);
		for (int i{}; i < N; i++)
			cin >> aliens[i];
		sort(aliens.begin(), aliens.end());
		vector<ll> pref(N, 0);
		pref[0] = aliens[0];
		for (int i{1}; i < N; i++)
			pref[i] = pref[i - 1] + aliens[i];

		cin >> Q;
		while (Q--)
		{
			ll p;
			cin >> p;
			// 1 2 2 2 2 3 4
			// upper_bound will include all 2s
			ll index = upper_bound(aliens.begin(), aliens.end(), p) - aliens.begin();

			if (index == 0)
				cout << -1 << endl;
			else
				cout << index << " " << pref[index - 1] << endl;
		}
	}
	return 0;
}