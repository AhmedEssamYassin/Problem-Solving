#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

bool isOk(const vector<ll> &a, ll S, ll K)
{
	vector<ll> temp = a;
	for (int i{}, j = 1; i < temp.size(); i++, j++)
		temp[i] += j * K;
	sort(temp.begin(), temp.end());
	vector<ll> pref{0};
	for (int i{}; i < temp.size(); i++)
		pref.push_back(pref.back() + temp[i]);
	ll cnt = lower_bound(pref.begin(), pref.end(), S) - pref.begin();
	if (pref[cnt] > S)
		cnt--;
	return cnt >= K;
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
	ll N, S;
	// cin >> t;
	while (t--)
	{
		cin >> N >> S;
		vector<ll> a(N);
		for (int i{}; i < N; i++)
			cin >> a[i];
		ll L{}, R = N, K = 0;
		while (L <= R)
		{
			ll mid = ((L + R) >> 1);
			if (isOk(a, S, mid))
				K = mid, L = mid + 1;
			else
				R = mid - 1;
		}
		for (int i{}, j = 1; i < a.size(); i++, j++)
			a[i] += j * K;
		sort(a.begin(), a.end());
		vector<ll> pref{0};
		for (int i{}; i < a.size(); i++)
			pref.push_back(pref.back() + a[i]);
		cout << K << " " << pref[K];
	}
	return 0;
}