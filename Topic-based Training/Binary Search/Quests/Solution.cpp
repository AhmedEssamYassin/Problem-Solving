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
	ll N, c, d;
	cin >> t;
	while (t--)
	{
		cin >> N >> c >> d;
		vector<ll> vc(N);
		for (int i{}; i < N; i++)
			cin >> vc[i];
		sort(vc.begin(), vc.end(), greater<ll>()); // Sorting in descending order

		ll L{}, R = d + 2;
		while (L < R)
		{
			ll mid = L + ((R - L + 1) >> 1);
			ll tot = 0;
			for (int i = 0; i < d; i++)
			{
				if (i % mid < N)
					tot += vc[i % mid];
			}
			if (tot >= c)
				L = mid; // Find a better (larger K)
			else
				R = mid - 1;
		}
		if (L == d + 2)
			cout << "Infinity\n";
		else if (L == 0)
			cout << "Impossible\n";
		else
			cout << L - 1 << "\n";
	}
	return 0;
}