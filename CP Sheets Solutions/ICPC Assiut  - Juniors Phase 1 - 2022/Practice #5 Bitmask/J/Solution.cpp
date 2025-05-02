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
	int t = 1;
	ll N, K;
	// cin >> t;
	while (t--)
	{
		cin >> N;
		vector<ll> a(N);
		ll bit[20]{};
		for (int i{}; i < N; i++)
		{
			cin >> a[i];
			for (int k{}; k < 20; k++)
			{
				if (a[i] & (1LL << k))
					bit[k]++;
			}
		}
		for (int k{}; k < 20; k++)
		{
			int i;
			for (i = N - 1; i > N - bit[k] - 1; i--)
				a[i] |= (1LL << k);
			for (NULL; i >= 0; i--)
				a[i] &= ~(1LL << k);
		}
		ll sum{};
		for (int i{}; i < N; i++)
			sum += (a[i] * a[i]);
		cout << sum;
	}
	return 0;
}