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
	ll N, S;
	cin >> t;
	while (t--)
	{
		cin >> N;
		vector<ll> a(N), prev;
		for (int i{}; i < N; i++)
			cin >> a[i];

		// a[i] < i < a[j] < j
		ll cnt{};
		// 1 1 2 3 8 2 1 4
		// 1 2 3 4 5 6 7 8
		// s 0 0 1 s 1 1 3
		for (int i{}; i < N; i++)
		{
			if (a[i] < i + 1)
			{
				// count how many a[j] > i exist so far
				cnt += lower_bound(prev.begin(), prev.end(), a[i]) - prev.begin();
				prev.push_back(i + 1);
			}
		}
		cout << cnt << endl;
	}
	return 0;
}