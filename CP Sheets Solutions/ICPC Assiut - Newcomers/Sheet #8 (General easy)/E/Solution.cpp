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
		ll p, q, cnt{};
		cin >> N;
		while (N--)
		{
			cin >> p >> q;
			if (q - p >= 2)
				cnt++;
		}
		cout << cnt;
	}
	return 0;
}
