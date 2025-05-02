#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

ll isOk(int arr[], int N, int h, int x)
{
	ll sum{};
	for (int i{}; i < N; i++)
	{
		if (h >= arr[i])
			sum += (h - arr[i]);
	}
	return (sum <= x);
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif
	int t;
	int N, x;
	cin >> t;

	while (t--)
	{
		cin >> N >> x;
		int *arr = new int[N];
		for (int i{}; i < N; i++)
			cin >> arr[i];

		ll L = 0, R = 2 * 1000000000;
		ll ans;
		while (L <= R)
		{
			ll mid = L + (R - L) / 2;

			if (isOk(arr, N, mid, x))
			{
				ans = mid;
				L = mid + 1;
			}
			else
				R = mid - 1;
		}
		cout << ans << endl;
		delete[] arr;
	}

	return 0;
}