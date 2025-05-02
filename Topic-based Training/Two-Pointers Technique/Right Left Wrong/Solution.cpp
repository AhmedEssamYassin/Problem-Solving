#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

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
		vector<ll> a(N), pref(N, 0);
		for (int i{}; i < N; i++)
			cin >> a[i];

		pref[0] = a[0];
		for (int i{1}; i < N; i++)
			pref[i] = pref[i - 1] + a[i];
		vector<ll> L, R;
		string str;
		cin >> str;
		for (int i{}; i < N; i++)
		{
			if (str[i] == 'L')
				L.push_back(i);
			else
				R.push_back(i);
		}
		ll maxVal = 0;
		for (int i{}, j = R.size() - 1; i < L.size() && j >= 0; i++, j--)
			maxVal += (pref[R[j]] - (L[i] > 0 ? pref[L[i] - 1] : 0)) * (R[j] >= L[i]);
		cout << maxVal << endl;
	}
	return 0;
}