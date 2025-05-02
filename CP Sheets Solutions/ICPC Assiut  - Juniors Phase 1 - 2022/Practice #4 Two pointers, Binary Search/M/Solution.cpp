#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

bool isOk(set<ll> st, const vector<ll> &towersCoord, ll r)
{
	for (const ll &xi : towersCoord)
	{
		// Remove any numbers in range [xi - r, xi + r] from the set
		auto it = st.lower_bound(xi - r);
		while (it != st.end() && *it <= xi + r)
			it = st.erase(it); // Erase and move iterator forward
	}
	return st.empty();
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
	ll N, M;
	// cin >> t;
	while (t--)
	{
		cin >> N >> M;
		vector<ll> citiesCoord(N), towersCoord(M);
		for (ll &xi : citiesCoord)
			cin >> xi;

		for (ll &xi : towersCoord)
			cin >> xi;
		// Both are in non-decreasing order
		ll L{}, R = (1LL << 32), r;
		set<ll> st(citiesCoord.begin(), citiesCoord.end()); // We don't care about cities located at the same coordinate (duplicates)
		while (L <= R)
		{
			ll mid = ((L + R) >> 1);
			if (isOk(st, towersCoord, mid))
				r = mid, R = mid - 1;
			else
				L = mid + 1;
		}
		cout << r;
	}
	return 0;
}