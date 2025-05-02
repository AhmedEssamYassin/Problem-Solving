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
	// cin >> t;
	while (t--)
	{
		cin >> N;
		vector<int> a(N);
		for (int i{}; i < N; i++)
			cin >> a[i];

		sort(a.begin(), a.end());

		int ans = 0;
		int L{}, R{};
		while (R < N)
		{
			while (a[R] - a[L] > 5)
				L++;
			// Now a[R] - a[L] <= 5
			ans = max(ans, R - L + 1);
			R++;
		}
		cout << ans << endl;
	}
	return 0;
}