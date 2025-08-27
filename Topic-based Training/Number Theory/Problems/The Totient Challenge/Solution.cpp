#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

template <typename T>
T binPow(T N, T power)
{
	T res = 1;
	while (power)
	{
		if (power & 1)
			res *= N;
		N *= N;
		power >>= 1;
	}
	return res;
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
	ll N;
	// cin >> t;
	while (t--)
	{
		cin >> N;
		ll cnt{};
		auto Log2 = [&](ll N)
		{
			ll res{};
			while (N > 1)
				N >>= 1, res++;
			return res;
		};
		cnt += 1; // The 1 itself
		int maxPower = Log2(N);
		for (ll a = 1; a <= maxPower; a++)
		{
			for (ll b = 0; b <= maxPower; b++)
				cnt += (binPow(2LL, a) <= N / binPow(3LL, b));
		}
		cout << cnt;
	}
	return 0;
}