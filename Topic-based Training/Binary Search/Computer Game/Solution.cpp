#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

bool isOk(ll K, ll N, ll a, ll b, ll x)
{
	return (K > x * a + (N - x) * b);
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
	int t = 1;
	ll N, K, a, b;
	cin >> t;
	while (t--)
	{
		cin >> K >> N >> a >> b;
		if (K - N * a > 0)
			cout << N << endl;
		else
		{
			int ans = -1;
			ll L{}, R{(1LL << 32)};
			while (L <= R)
			{
				ll mid = ((L + R) >> 1);
				if (isOk(K, N, a, b, mid))
				{
					ans = mid;
					L = mid + 1;
				}
				else
					R = mid - 1;
			}
			cout << ans << endl;
		}
	}
	return 0;
}