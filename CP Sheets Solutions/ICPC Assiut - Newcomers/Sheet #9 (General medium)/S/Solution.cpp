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
	int t = 1;
	ll N, M;
	// cin >> t;
	while (t--)
	{
		cin >> N;
		vector<ll> arr(N);
		for (int i{}; i < N; i++)
			cin >> arr[i];

		sort(arr.begin(), arr.end());

		int L{0}, R{2};
		while (R < N)
		{
			if (arr[L] + arr[L + 1] > arr[R])
				return cout << "YES", 0;
			L++, R++;
		}
		cout << "NO";
	}
	return 0;
}
