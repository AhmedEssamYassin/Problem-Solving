#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

vector<ll> factors[100001];
void factorize(int N)
{
	for (ll d = 1; d <= N; d++)
	{
		for (ll m = d; m <= N; m += d)
			factors[m].push_back(d);
	}
}
static int autoCall = (factorize(100000), 0);

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
	int t = 1;
	ll N, Q;
	cin >> t;
	while (t--)
	{
		cin >> N >> Q;
		vector<ll> a(N);
		for (int i{}; i < N; i++)
			cin >> a[i];
		vector<ll> pos[100001];
		for (int i = 0; i < N; i++)
			pos[a[i]].push_back(i);

		auto findNextDivisor = [&](int k, int l) -> int
		{
			int next = N;
			for (const ll &d : factors[k])
			{
				if (d == 1)
					continue;

				auto it = lower_bound(pos[d].begin(), pos[d].end(), l);

				if (it != pos[d].end())
					next = min<ll>(next, *it);
			}

			return next;
		};

		while (Q--)
		{
			ll k, l, r;
			cin >> k >> l >> r;
			l--, r--;
			ll ans{};
			while (l <= r)
			{
				ll next = min<ll>(findNextDivisor(k, l), r + 1);
				ans += (next - l) * k;
				l = next;

				while (l <= r && k % a[l] == 0)
					k /= a[l];
			}
			cout << ans << endl;
		}
	}
	return 0;
}