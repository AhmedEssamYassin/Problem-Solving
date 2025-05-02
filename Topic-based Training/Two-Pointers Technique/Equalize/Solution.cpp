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
	ll N;
	cin >> t;
	while (t--)
	{
		cin >> N;
		vector<ll> vc(N);
		for (int i{}; i < N; i++)
			cin >> vc[i];
		sort(vc.begin(), vc.end());
		vc.erase(unique(vc.begin(), vc.end()), vc.end());
		ll cnt{}, L{}, R{};
		while (R < vc.size())
		{
			while (vc[R] - vc[L] >= N)
				L++;
			cnt = max(cnt, R - L + 1);
			R++;
		}
		cout << cnt << endl;
	}
	return 0;
}