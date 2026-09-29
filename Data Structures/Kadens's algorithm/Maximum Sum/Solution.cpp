#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

const ll mod = 1e9 + 7;

#define double_size_t std::conditional_t<(mod > (1LL << 31)), __int128_t, long long>
inline ll add64(const ll &a, const ll &b)
{
	double_size_t res = double_size_t(a) + b;
	if (res >= mod)
		res -= mod;
	return res;
}

inline ll sub64(const ll &a, const ll &b)
{
	double_size_t res = double_size_t(a) - b;
	if (res < 0)
		res += mod;
	if (res >= mod)
		res -= mod;
	return res;
}

inline ll mult64(const ll &a, const ll &b)
{
	return double_size_t(a) * b % mod;
}

ll modPow(ll N, ll power)
{
	if (N % mod == 0 || N == 0)
		return 0;
	if (N == 1 || power == 0)
		return 1;

	ll res{1};
	while (power)
	{
		if (power & 1)
			res = mult64(res, N);
		N = mult64(N, N);
		power >>= 1;
	}
	return res;
}

// Kadane: max sum of a non-empty subarray. O(n)
ll maxSubarraySum(const vector<ll> &a)
{
	ll mx = LLONG_MIN, cur = 0;
	for (const ll &x : a)
	{
		cur = max(cur, 0LL) + x;
		mx = max(mx, cur);
	}
	return mx;
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
	cin >> t;
	ll N, K;
	while (t--)
	{
		cin >> N >> K;
		vector<ll> arr(N);
		for (int i{}; i < N; i++)
			cin >> arr[i];

		ll maxSum = max(0LL, maxSubarraySum(arr));

		ll S{};
		for (int i{}; i < N; i++)
			S += arr[i];
		ll val = sub64(modPow(2, K), 1);
		ll ans = add64((S % mod + mod) % mod, mult64(val, maxSum % mod));
		cout << ans << endl;
	}
	return 0;
}