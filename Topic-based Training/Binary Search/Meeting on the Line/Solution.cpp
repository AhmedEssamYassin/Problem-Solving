#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

pair<ll, ll> dist(const vector<ll> &X, const vector<ll> &dressTime, ll Time)
{
	ll maxStart = 0, minEnd = LLONG_MAX;
	for (int i{}; i < X.size(); i++)
	{
		if (dressTime[i] > Time)
		{
			maxStart = LLONG_MAX, minEnd = 0;
			break;
		}
		maxStart = max(maxStart, X[i] - (Time - dressTime[i]));
		minEnd = min(minEnd, X[i] + (Time - dressTime[i]));
	}
	return {maxStart, minEnd};
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
	ll N;
	cin >> t;
	while (t--)
	{
		cin >> N;
		vector<ll> X(N), T(N);
		for (int i{}; i < N; i++)
			cin >> X[i];
		for (int i{}; i < N; i++)
			cin >> T[i];

		ll L{}, R = 1e8;
		ll ans{};
		while (L <= R)
		{
			ll mid = ((L + R) >> 1);
			pair<ll, ll> cur = dist(X, T, mid);
			if (cur.first <= cur.second)
			{
				ans = mid;	 // A candidate solution
				R = mid - 1; // Search for a better (smaller) answer
			}
			else
				L = mid + 1;
		}
		ll maxStart = 0, minEnd = LLONG_MAX;
		for (int i{}; i < N; i++)
		{
			maxStart = max(maxStart, X[i] - (ans - T[i]));
			minEnd = min(minEnd, X[i] + (ans - T[i]));
		}
		cout << fixed << setprecision(9) << 0.5 * (maxStart + minEnd) << endl;
	}
	return 0;
}