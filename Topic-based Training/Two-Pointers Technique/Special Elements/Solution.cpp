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
	ll N;
	cin >> t;
	while (t--)
	{
		cin >> N;
		int *arr = new int[N];
		for (int i{}; i < N; i++)
			cin >> arr[i];

		int cnt{};
		for (int i{}; i < N; i++)
		{
			int L = {}, R{}; // Starting at index 0
			ll sum{};
			int curr = arr[i];
			while (R < N)
			{
				sum += arr[R]; // Expand
				while (sum > curr && L <= R)
					sum -= arr[L++]; // Shrink

				if (sum == curr && R - L + 1 >= 2)
				{
					cnt++;
					break;
				}
				R++;
			}
		}
		cout << cnt << endl;
		delete[] arr;
	}
	return 0;
}