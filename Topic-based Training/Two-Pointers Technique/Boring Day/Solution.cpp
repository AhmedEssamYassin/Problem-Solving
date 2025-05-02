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
	ll N, L, R;
	cin >> t;
	while (t--)
	{
		cin >> N >> L >> R;
		vector<ll> a(N);
		for (int i{}; i < N; i++)
			cin >> a[i];
		// 2 1 11 3 7
		ll cnt{}, sum{}, left{}, right{};
		while (right < N)
		{
			sum += a[right];
			if (sum >= L && sum <= R)
				sum = 0, cnt++, left = right + 1;
			while (sum > R && left <= right)
			{
				sum -= a[left++];
				if (sum >= L && sum <= R)
				{
					sum = 0, cnt++, left = right + 1;
					break;
				}
			}
			right++;
		}
		cout << cnt << endl;
	}
	return 0;
}