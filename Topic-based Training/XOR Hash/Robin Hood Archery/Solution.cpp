#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

using u64 = uint64_t;
u64 S = chrono::steady_clock::now().time_since_epoch().count() ^ (u64) new char;
u64 rnd()
{
	S += 0xa0761d6478bd642f;
	__uint128_t t = (__uint128_t)S * (S ^ 0xe7037ed1a0b428db);
	return t >> 64 ^ t;
}
#define rng(l, r) ((l) + (int64_t)((__uint128_t)rnd() * ((r) - (l) + 1) >> 64))

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif
	int t = 1;
	ll N, Q;
	cin >> t;
	vector<ll> XOR(1e6 + 1);
	for (ll &hash : XOR)
		hash = rng(1LL << 40, 1LL << 63);
	while (t--)
	{
		cin >> N >> Q;
		vector<ll> a(N);
		for (int i{}; i < N; i++)
			cin >> a[i];

		vector<ll> pref(N, 0);
		pref[0] = XOR[a[0]];
		for (int i{1}; i < N; i++)
			pref[i] = (pref[i - 1] ^ XOR[a[i]]);
		while (Q--)
		{
			ll L, R;
			cin >> L >> R;
			L--, R--;
			ll Round = pref[R] ^ (L ? pref[L - 1] : 0);
			if (Round == 0)
				cout << "YES\n";
			else
				cout << "NO\n";
		}
	}
	return 0;
}