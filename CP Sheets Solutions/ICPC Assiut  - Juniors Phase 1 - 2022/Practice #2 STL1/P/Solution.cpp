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
		cin >> N >> Q;
		deque<ll> deq(N);
		for (int i{}; i < N; i++)
			cin >> deq[i];
		vector<pair<ll, ll>> ans;
		ll mx = *max_element(deq.begin(), deq.end());
		while (mx != deq[0])
		{
			ll first = deq[0];
			ll second = deq[1];
			deq.pop_front();
			deq.pop_front();
			deq.push_front(max(first, second));
			deq.push_back(min(first, second));
			ans.push_back({first, second});
		}
		while (Q--)
		{
			ll m;
			cin >> m;
			if (m <= ans.size())
			{
				cout << ans[m - 1].first << " " << ans[m - 1].second << endl;
				continue;
			}
			m -= ans.size();
			m %= (N - 1);
			// The deque now is [5, 1, 2, 3, 4]
			// We need cycles of [1, 2, 3, 4], so we need indices 1, 2, 3, 4
			// So if m % (N - 1) is 0 that means we are at the fifth element which is deq[N - 1]
			if (m == 0)
				m = N - 1;
			cout << mx << " " << deq[m] << endl;
		}
	}
	return 0;
}