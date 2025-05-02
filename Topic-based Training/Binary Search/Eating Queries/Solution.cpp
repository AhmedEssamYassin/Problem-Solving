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
	int t, N, Q, X;
	cin >> t;
	while (t--)
	{
		cin >> N >> Q;
		vector<int> arr(N), prefSum(N);
		for (int i{}; i < N; i++)
			cin >> arr[i];

		sort(arr.begin(), arr.end(), greater<>());

		prefSum[0] = arr[0];
		for (int i{1}; i < N; i++)
			prefSum[i] = prefSum[i - 1] + arr[i];

		while (Q--)
		{
			cin >> X;
			vector<int>::iterator it = lower_bound(prefSum.begin(), prefSum.end(), X);
			if (it != prefSum.end())
				cout << (it - prefSum.begin() + 1) << endl;
			else
				cout << -1 << endl;
		}
	}
	return 0;
}