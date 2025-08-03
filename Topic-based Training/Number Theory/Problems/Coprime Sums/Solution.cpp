#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

vector<int> mu;
vector<vector<int>> divisors;
void precompute(int N)
{
	mu.resize(N + 1);
	divisors.resize(N + 1);
	mu[1] = -1;
	for (int i = 1; i <= N; i++)
	{
		if (mu[i])
		{
			mu[i] = -mu[i];
			for (int j = 2 * i; j <= N; j += i)
				mu[j] += mu[i];
		}
	}
	for (int i{1}; i <= N; i++)
	{
		for (ll j = i; j <= N; j += i)
			divisors[j].push_back(i);
	}
}

static int autoCall = (precompute(2e5), 0);

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
	int t = 1;
	ll N, p;
	cin >> t;
	while (t--)
	{
		cin >> N >> p;
		vector<ll> a(N + 1);
		for (int i{1}; i <= N; i++)
			cin >> a[i];

		vector<ll> pre(N + 1, 0);
		for (int i{2}; i <= N; i++)
		{
			for (ll j = i; j <= N; j += i)
				pre[i] += a[j];
		}
		ll sum = accumulate(a.begin(), a.end(), 0LL);
		ll cur = sum;
		for (int i{}; i < p; i++)
		{
			ll k;
			cin >> k;
			cur = sum;
			for (const ll &d : divisors[k])
				cur += mu[d] * pre[d];
			cout << cur << " ";
		}
		cout << endl;
	}
	return 0;
}