#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

constexpr ll comb(ll N)
{
	return N * (N - 1) / 2;
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
	ll N, x, m, k;
	cin >> t;
	while (t--)
	{
		cin >> N;
		m = 3, k = 2;
		vector<ll> vc(N);
		for (int i{}; i < N; i++)
			cin >> vc[i];
		sort(vc.begin(), vc.end());
		ll ans{};
		// 2 2 3 3 3 3 4 4 4 5 5 5 5 5
		for (int L = 1; L <= N; L++)
		{
			int R = upper_bound(vc.begin(), vc.end(), vc[L - 1] + k) - vc.begin();
			ans = (ans + comb(R - L));
		}
		cout << ans << endl;
	}
	return 0;
}