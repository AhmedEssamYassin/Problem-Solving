#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

ll l, r;
bool isOk(const vector<ll> &vc, ll len, ll k)
{
	bool check = false;
	ll cnt0{}, cnt1{};
	ll N = vc.size();
	for (int i{}; i < len; i++)
	{
		if (vc[i] == 0)
			cnt0++;
		else
			cnt1++;
	}
	if (k >= cnt0)
	{
		l = 0, r = len - 1;
		return true;
	}

	for (int i = len, j = 0; i < N; i++, j++)
	{
		if (vc[i] == 0)
			cnt0++;
		else
			cnt1++;

		if (vc[j] == 0)
			cnt0--;
		else
			cnt1--;
		if (k >= cnt0)
		{
			l = j + 1, r = i;
			return true;
		}
	}
	return false;
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
	ll N, k;
	// cin >> t;
	while (t--)
	{
		cin >> N >> k;
		vector<ll> vc(N);
		for (int i{}; i < N; i++)
			cin >> vc[i];
		ll L{}, R = N, ans{};
		while (L <= R)
		{
			ll mid = (L + R) / 2;
			if (isOk(vc, mid, k))
				ans = mid, L = mid + 1;
			else
				R = mid - 1;
		}
		cout << ans << endl;
		replace(vc.begin() + l, vc.begin() + r + 1, 0, 1);
		for (const ll &x : vc)
			cout << x << " ";
	}
	return 0;
}