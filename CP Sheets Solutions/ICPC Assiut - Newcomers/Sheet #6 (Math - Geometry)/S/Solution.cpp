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
	ll N;
	// cin >> t;
	while (t--)
	{
		ll arr[3];
		cin >> arr[0] >> arr[1] >> arr[2];
		sort(arr, arr + 3);
		if (arr[0] + arr[1] <= arr[2])
			cout << "Invalid";
		else
		{
			cout << "Valid" << endl;
			long double per = arr[0] + arr[1] + arr[2];
			per /= 2.0;
			cout << fixed << setprecision(6) << sqrtl(per * (per - arr[0]) * (per - arr[1]) * (per - arr[2]));
		}
	}
	return 0;
}