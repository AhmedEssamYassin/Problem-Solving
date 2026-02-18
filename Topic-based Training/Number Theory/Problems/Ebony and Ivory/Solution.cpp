#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct LinearDiophantineEqn
{
	ll x0, y0, g, a, b;

	ll floorDiv(ll a, ll b)
	{
		return a / b - ((a ^ b) < 0 && a % b);
	}

	ll ceilDiv(ll a, ll b)
	{
		return a / b + ((a ^ b) > 0 && a % b);
	}

	ll extendedGcd(ll a, ll b, ll &x, ll &y)
	{
		if (b == 0)
		{
			x = 1;
			y = 0;
			return a;
		}
		ll x1, y1;
		ll d = extendedGcd(b, a % b, x1, y1);
		x = y1;
		y = x1 - y1 * (a / b);
		return d;
	}

	// Solves ax + by = c. Returns false if impossible.
	bool solve(ll _a, ll _b, ll c)
	{
		a = _a;
		b = _b;
		if (a == 0 && b == 0)
		{
			g = 0;
			return c == 0; // Infinite solutions if c=0, else 0
		}
		g = extendedGcd(abs(a), abs(b), x0, y0);
		if (c % g)
			return false;

		x0 *= c / g;
		y0 *= c / g;
		if (a < 0)
			x0 = -x0;
		if (b < 0)
			y0 = -y0;
		return true;
	}

	bool hasNonNegative(ll minX, ll maxX, ll minY, ll maxY)
	{
		// Use 2e18 as logical infinity for "unbounded" upper limit
		return countSolutions(minX, maxX, minY, maxY) > 0;
	}
	// Returns solution {x, y} for a specific integer k
	// x = x0 + k * (b / g), y = y0 - k * (a / g)
	pair<ll, ll> getSolution(ll k)
	{
		return {x0 + k * (b / g), y0 - k * (a / g)};
	}

	ll countSolutions(ll minX, ll maxX, ll minY, ll maxY)
	{
		if (a == 0 && b == 0)
			return (g == 0) ? -1 : 0; // -1 indicates infinite
		if (a == 0)
			return (minY <= y0 && y0 <= maxY) ? (maxX - minX + 1) : 0;
		if (b == 0)
			return (minX <= x0 && x0 <= maxX) ? (maxY - minY + 1) : 0;

		ll dx = b / g;
		ll dy = a / g;

		ll kMinX = (dx > 0) ? ceilDiv(minX - x0, dx) : ceilDiv(maxX - x0, dx);
		ll kMaxX = (dx > 0) ? floorDiv(maxX - x0, dx) : floorDiv(minX - x0, dx);

		ll kMinY = (dy > 0) ? ceilDiv(y0 - maxY, dy) : ceilDiv(y0 - minY, dy);
		ll kMaxY = (dy > 0) ? floorDiv(y0 - minY, dy) : floorDiv(y0 - maxY, dy);

		ll kStart = max(kMinX, kMinY);
		ll kEnd = min(kMaxX, kMaxY);

		return (kStart > kEnd) ? 0 : (kEnd - kStart + 1);
	}
};

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif
	int t = 1;
	// cin >> t;
	while (t--)
	{
		ll a, b, c;
		cin >> a >> b >> c;
		LinearDiophantineEqn ds;
		if (ds.solve(a, b, c) && ds.hasNonNegative(0, 10000, 0, 10000))
			cout << "Yes";
		else
			cout << "No";
	}
	return 0;
}