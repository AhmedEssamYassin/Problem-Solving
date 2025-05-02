#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

bool isOk(const vector<ll> &a, const vector<ll> &b, const vector<ll> &pref, const vector<ll> &suff, ll val)
{
	for (int i = 0; i <= a.size(); i++)
		if (pref[i] + (pref[i] < b.size() && val >= b[pref[i]]) + suff[i] >= b.size())
			return true;

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
	ll N, M;
	cin >> t;
	while (t--)
	{
		cin >> N >> M;
		vector<ll> a(N), b(M);
		for (int i{}; i < N; i++)
			cin >> a[i];
		for (int i{}; i < M; i++)
			cin >> b[i];
		vector<ll> pref(N + 1, 0), suff(N + 2, 0);
		int j = 0;
		for (int i = 0; i < N; i++)
		{
			if (j < M && a[i] >= b[j])
				j++;
			pref[i + 1] = j;
		}
		j = M - 1;
		for (int i = N - 1; i >= 0; i--)
		{
			if (j >= 0 && a[i] >= b[j])
				j--;
			suff[i] = M - 1 - j;
		}
		if (pref[N] == M)
			cout << 0 << endl;
		else
		{
			vector<ll> vc = b;
			sort(vc.begin(), vc.end());

			ll ans = -1;
			ll L = 0, R = vc.size() - 1;
			while (L <= R)
			{
				ll mid = ((L + R) >> 1);
				if (isOk(a, b, pref, suff, vc[mid]))
					ans = vc[mid], R = mid - 1;
				else
					L = mid + 1;
			}
			cout << ans << endl;
		}
	}
	return 0;
}