#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

int calcTeams(int *arr, int N, int S)
{
	int L{};
	int R{N - 1};
	int cnt{};
	while (L < R)
	{
		if (arr[L] + arr[R] == S)
		{
			cnt++;
			L++, R--;
		}
		else if (arr[L] + arr[R] < S)
			L++;
		else
			R--;
	}
	return cnt;
}

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

		sort(arr, arr + N);

		int L{2}, R{101}; // For optimal S
		int ans = 0;
		while (L <= R)
		{
			int candidate = calcTeams(arr, N, L);
			ans = max(ans, candidate);
			L++;
		}
		cout << ans << endl;
		delete[] arr;
	}
	return 0;
}