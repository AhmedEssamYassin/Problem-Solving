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
	int t = 1, N;
	// cin >> t;
	while (t--)
	{
		cin >> N;
		int *arr = new int[N];
		for (int i{}; i < N; i++)
			cin >> arr[i];
		// This solution works only for non-negative numbers
		int L{}, R{N - 1};
		ll sum_L{}, sum_R{arr[R--]}, ans{};
		while (L <= R)
		{
			sum_L += arr[L];
			while (sum_R < sum_L && L < R)
				sum_R += arr[R--];

			if (sum_L == sum_R)
				ans = max(ans, sum_L);
			L++;
		}
		cout << ans;
		delete[] arr;
	}
	return 0;
}