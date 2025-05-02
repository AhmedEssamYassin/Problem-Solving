#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

const ll mod = 1e9 + 7;
ll fact[200005], invFact[200005];

ll modPow(ll N, ll power, ll mod)
{
	ll res{1};
	while (power)
	{
		if (power & 1)
			res = (res % mod * N % mod) % mod;
		N = (N % mod * N % mod) % mod;
		power >>= 1;
	}
	return res;
}

ll comb(ll N, ll R)
{
	if (N < R)
		return 0;
	return (fact[N] % mod * invFact[N - R] % mod * invFact[R] % mod) % mod;
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
	ll N, x, m, k;
	fact[0] = 1;
	for (int i = 1; i <= 200005; i++)
		fact[i] = (fact[i - 1] * i) % mod;
	for (int i{}; i <= 200005; i++)
		invFact[i] = modPow(fact[i], mod - 2, mod);
	cin >> t;
	while (t--)
	{
		cin >> N >> m >> k;
		vector<ll> vc(N);
		for (int i{}; i < N; i++)
			cin >> vc[i];
		sort(vc.begin(), vc.end());
		ll ans{};
		// Fix one, choose 2
		// 2 2 3 3 3 3 4 4 4 5 5 5 5 5
		for (int L = 1; L <= N; L++)
		{
			int R = upper_bound(vc.begin(), vc.end(), vc[L - 1] + k) - vc.begin();
			ans = (ans + comb(R - L, m - 1)) % mod;
		}
		cout << ans << endl;
	}
	return 0;
}