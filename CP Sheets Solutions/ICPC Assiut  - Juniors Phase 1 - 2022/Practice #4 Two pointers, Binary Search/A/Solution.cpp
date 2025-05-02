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
	ll N, time;
	// cin >> t;
	while (t--)
	{
		cin >> N >> time;
		vector<ll> a(N);
		for (int i{}; i < N; i++)
			cin >> a[i];
		// 3 1 2 1
		ll L{}, R{};
		ll curSum{};
		ll cnt{};
		ll maxCnt{};
		while (R < N)
		{
			curSum += a[R];
			if (curSum <= time)
				cnt++;
			else
			{
				maxCnt = max(maxCnt, cnt);
				while (curSum > time)
					curSum -= a[L++], cnt--;
				cnt++; // Counts the element who caused curSum to exceed time
			}
			if (R == N - 1)
				maxCnt = max(maxCnt, cnt);
			R++;
		}
		cout << maxCnt;
	}
	return 0;
}