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

typedef array<int, 30> number; // Max number is 5e5 < (1 << 30)

ll toInt(const number &x)
{
	return accumulate(x.begin(), x.end(), 0LL);
}

number operator^(const number &LHS, const number &RHS)
{
	number C;
	for (int i{}; i < 30; i++)
	{
		C[i] = LHS[i] + RHS[i];
		C[i] -= 3 * (C[i] >= 3);
	}
	return C;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif
	int t = 1;
	ll N;
	// cin >> t;
	while (t--)
	{
		cin >> N;
		vector<ll> a(N);
		for (int i{}; i < N; i++)
			cin >> a[i];

		vector<number> vc(N + 1);
		// setting random values to all numbers
		for (int i{1}; i <= N; i++)
			for (int j{}; j < 30; j++)
				vc[i][j] = rnd() % 3;

		vector<number> pref(1);
		vector<vector<int>> pos(N + 1);
		map<number, int> cnt;
		cnt[array<int, 30>{0}] = 1;
		int cur = 0;
		ll ans = 0;
		for (int i{}; i < N; i++)
		{
			pref.push_back((pref.back() ^ vc[a[i]]));
			pos[a[i]].push_back(i);
			// 1 2 2 2 1 1 2 2 2
			if (pos[a[i]].size() >= 4)
			{
				while (cur <= pos[a[i]][pos[a[i]].size() - 4])
				{
					cnt[pref[cur]]--;
					++cur;
				}
			}
			ans += cnt[pref.back()];
			cnt[pref.back()]++;
		}
		cout << ans;
	}
	return 0;
}