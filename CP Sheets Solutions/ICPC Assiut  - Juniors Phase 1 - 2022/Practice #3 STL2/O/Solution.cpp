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
		vector<ll> arr(N);
		for (int i{}; i < N; i++)
			cin >> arr[i];

		/*
		The problem is that when negative numbers are present, your approach may find incorrect matches. This happens because:
		A prefix sum and a suffix sum could be equal, but the middle section might not form a valid contiguous subarray
		*/
		ll pref{};
		map<ll, vector<int>> mp; // {prefixSum, index}
		for (int i = 0; i < N; i++)
			mp[pref += arr[i]].push_back(i);

		ll ans = LLONG_MIN;

		// Try all possible third segments
		ll suffix_sum = 0;
		for (int i = N - 1; i >= 0; i--)
		{
			// Starting from the end
			suffix_sum += arr[i];
			// For each potential first segment ending (first part sum must equal third part sum)
			if (mp.count(suffix_sum))
			{
				auto &vc = mp[suffix_sum];
				int j = lower_bound(vc.begin(), vc.end(), i) - vc.begin();
				if (j > 0)
					j--;
				if (vc[j] < i)
					ans = max(ans, suffix_sum);
			}
		}
		cout << ans;
	}
	return 0;
}