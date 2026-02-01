#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

bool isPerfectSquare(ll x)
{
	ll sq = sqrt(x);
	return (sq * sq == x);
}

int minNumSquares(ll x)
{
	if (isPerfectSquare(x))
		return 1;

	for (ll i = 1; i * i <= x; ++i)
	{
		ll rem = x - i * i;
		if (isPerfectSquare(rem))
			return 2;
	}
	// Legendre’s Three Square Theorem
	while (x % 4 == 0)
		x /= 4;
	if (x % 8 == 7)
		return 4;

	return 3;
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
	cin >> t;
	while (t--)
	{
		ll n, x;
		cin >> x >> n;
		int y = minNumSquares(x);
		if (y > n)
			cout << "NO\n";
		else
			cout << "YES\n";
	}
	return 0;
}
