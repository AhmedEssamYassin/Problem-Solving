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
	ll N, a, b;
	// cin >> t;
	while (t--)
	{
		cin >> a >> b;
		cout << gcd(a, b) << " " << lcm(a, b);
	}
	return 0;
}