#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

ll nCr(ll n, ll r)
{
	if (n == r || r == 0)
		return 1;
	return n * nCr(n - 1, r - 1) / r;
}

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
		cin >> N;
		for (int i{}; i < N; i++)
		{
			for (int j = 0; j <= i; j++)
				cout << nCr(i, j) << " \n"[j == i];
		}
	}
	return 0;
}