#include <bits/stdc++.h>
using namespace std;
#define ll long long int
// #define endl "\n"

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
		auto Ask = [&](int i, int j)
		{
			cout << "? " << i << " " << j;
			cout << endl;
			ll x;
			cin >> x;
			return x;
		};
		cin >> N;
		vector<int> ans(N + 1);
		/*
		if a[i] % a[j] < a[j] % a[i] implies a[i] > a[j]
		if a[i] % a[j] > a[j] % a[i] implies a[i] < a[j]
		if a[i] < a[j] then, a[i] % a[j] = a[i]
		*/
		int posMax = 1;
		for (int i = 2; i <= N; i++)
		{
			int mod_ij = Ask(i, posMax);
			int mod_ji = Ask(posMax, i);
			if (mod_ij < mod_ji)
			{
				ans[posMax] = mod_ji;
				posMax = i;
			}
			else
				ans[i] = mod_ij;
		}
		ans[posMax] = N;
		cout << "! ";
		for (int i = 1; i <= N; i++)
			cout << ans[i] << " ";
	}
	return 0;
}