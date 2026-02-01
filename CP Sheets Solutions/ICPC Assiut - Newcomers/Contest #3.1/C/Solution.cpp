#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif
	int t = 1, N, K;
	// cin >> t;
	while (t--)
	{
		cin >> N >> K;
		int *arr = new int[N];
		for (int i{}; i < N; i++)
			cin >> arr[i];
		sort(arr, arr + N, greater<>());
		ll max_Sum{};
		for (int i{}; i < N && K > 0; i++)
		{
			if (arr[i] > 0)
				max_Sum += arr[i], --K;
		}
		cout << max_Sum;
		delete[] arr;
	}
	return 0;
}