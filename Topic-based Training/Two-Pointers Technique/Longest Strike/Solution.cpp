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
	int t;
	ll N, K, L, R;
	cin >> t;
	while (t--)
	{
		cin >> N >> K;
		vector<int> arr(N), good;
		map<int, int> mp;
		for (int i{}; i < N; i++)
		{
			cin >> arr[i];
			mp[arr[i]]++;
		}

		for (const auto &[x, freq] : mp)
		{
			if (freq >= K)
				good.push_back(x);
		}

		if (good.empty())
			cout << -1 << endl;
		else
		{
			sort(arr.begin(), arr.end());
			int maxVal = 0;
			pair<int, int> res(good[0], good[0]);
			int L = good[0];
			for (int i = 1; i < good.size(); i++)
			{
				if (good[i] - 1 == good[i - 1])
				{
					if (good[i] - L > maxVal)
					{
						res = {L, good[i]};
						maxVal = good[i] - L;
					}
				}
				else
					L = good[i];
			}
			cout << res.first << " " << res.second << endl;
		}
	}
	return 0;
}