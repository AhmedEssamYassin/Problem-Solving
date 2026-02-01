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
		ll b, d, size, total{};
		cin >> N >> b >> d;
		int cnt{};
		while (N--)
		{

			cin >> size;
			if (size <= b)
				total += size;
			if (total > d)
			{
				cnt++;
				total = 0;
			}
		}
		cout << cnt;
	}
	return 0;
}
