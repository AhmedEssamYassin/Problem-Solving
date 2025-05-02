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
	ll N, Q, K, a;
	cin >> t;
	while (t--)
	{
		cin >> N >> Q;
		vector<int> height(N), leftmostMax(N);

		vector<ll> steps(N);

		cin >> height[0];
		steps[0] = height[0];
		leftmostMax[0] = height[0];

		for (int i{1}; i < N; i++)
		{
			cin >> height[i];

			leftmostMax[i] = max(leftmostMax[i - 1], height[i]);

			steps[i] = steps[i - 1] + height[i];
		}
		// 1 2 1 5
		// 1 2 2 5
		// 1 3 4 9
		// 1 2 4 9 10
		while (Q--)
		{
			cin >> K;

			int pos = upper_bound(leftmostMax.begin(), leftmostMax.end(), K) - leftmostMax.begin(); // First greater height

			if (pos - 1 < 0)
				cout << 0 << " ";
			else if (pos >= N)
				cout << steps.back() << " ";
			else
				cout << steps[pos - 1] << " ";
		}
		cout << endl;
	}
	return 0;
}