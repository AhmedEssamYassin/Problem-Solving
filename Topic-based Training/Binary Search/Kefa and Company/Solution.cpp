#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct Friend
{
	ll money, friendship;
	bool operator<(const Friend &other)
	{
		return money < other.money;
	}
};

int findFarthest(const vector<Friend> &vc, int cur, int d)
{
	int L = 0, R = vc.size() - 1, j = -1;
	while (L <= R)
	{
		int mid = ((L + R) >> 1);
		if (vc[mid].money - cur < d)
			j = mid, L = mid + 1;
		else
			R = mid - 1;
	}
	return j;
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
	ll N, d;
	// cin >> t;
	while (t--)
	{
		cin >> N >> d;
		vector<Friend> vc(N);
		for (int i{}; i < N; i++)
			cin >> vc[i].money >> vc[i].friendship;
		sort(vc.begin(), vc.end());
		vector<ll> pref(N, 0);
		pref[0] = vc[0].friendship;
		for (int i{1}; i < N; i++)
			pref[i] = pref[i - 1] + vc[i].friendship;
		ll maxAns = LLONG_MIN;
		for (int i{}; i < N; i++)
		{
			int idx = findFarthest(vc, vc[i].money, d);
			ll currentVal = pref[idx] - (i ? pref[i - 1] : 0);
			maxAns = max(maxAns, currentVal);
		}
		cout << maxAns;
	}
	return 0;
}