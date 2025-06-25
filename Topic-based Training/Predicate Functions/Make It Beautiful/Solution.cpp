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
	ll N, k;
	cin >> t;
	while (t--)
	{
		cin >> N >> k;
		vector<ll> a(N);
		for (int i = 0; i < N; i++)
			cin >> a[i];
		auto pred = [&](const ll &x, const ll &y) -> bool
		{
			ll need_x{}, need_y{};
			for (int j = 0; j <= 63; j++)
			{
				if (((x >> j) & 1) == 0)
				{
					need_x = (x | ((1LL << (j + 1)) - 1)) - x;
					break;
				}
			}
			for (int j = 0; j <= 63; j++)
			{
				if (((y >> j) & 1) == 0)
				{
					need_y = (y | ((1LL << (j + 1)) - 1)) - y;
					break;
				}
			}
			return need_x > need_y;
		};

		priority_queue<ll, vector<ll>, function<bool(const ll &, const ll &)>> prQue(pred);
		for (int i = 0; i < N; i++)
			prQue.push(a[i]);
		ll ans = 0;
		while (!prQue.empty())
		{
			ll curr = prQue.top();
			prQue.pop();
			bool pushed = false;
			for (int j = 0; j <= 63; j++)
			{
				if (((curr >> j) & 1) == 0)
				{
					ll na = curr | ((1LL << (j + 1)) - 1);
					ll need = na - curr;
					if (need > 0 && need <= k)
					{
						curr = na;
						k -= need;
						prQue.push(curr);
						pushed = true;
						break;
					}
				}
			}
			if (!pushed)
				ans += __popcount(curr);
		}
		cout << ans << endl;
	}
	return 0;
}