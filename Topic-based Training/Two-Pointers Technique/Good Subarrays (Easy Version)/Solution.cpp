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
		vector<ll> a(N);
		for (int i{}; i < N; i++)
			cin >> a[i];

		ll cnt{}, j{};
		for (int i{}; i < N; i++)
		{
			cnt += min(i - j + 1, a[i]);
			j = max(j, i - a[i] + 1);
		}
		cout << cnt << endl;
	}
	return 0;
}