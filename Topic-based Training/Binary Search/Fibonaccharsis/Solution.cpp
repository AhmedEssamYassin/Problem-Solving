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
	ll N, K;
	cin >> t;
	while (t--)
	{
		cin >> N >> K;
		ll fib[30];
		fib[0] = 0;
		fib[1] = 1;
		for (int i{2}; i < 30; i++)
			fib[i] = fib[i - 1] + fib[i - 2];
		/*
		a b ... kth = N
		22 4
		6 8 14 22
		4 9 13 22
		2 10 12 22
		0 11 11 22
		a+0b 0a+b a+b a+2b 2a+3b 3a+5b 5a+8b + 8a+13b
		 0, 1, 1, 2, 3, 5, 8, 13, 21, 34
		f[i] * a + f[i + 1] * b = N
		kth is kth - 2 here
		f[2]*a + f[3]*b = a + 2 b = 22
		a = t --> b = (22 - t) / 2
		a = t --> b = (N - f[i] * t) / f[i + 1]
		(N - f[i] * t) = X * f[i + 1]
		*/
		if (K > 28)
		{
			cout << 0 << endl;
			continue;
		}
		K -= 2;
		ll ans{};
		for (int t{}; t <= N / fib[K] + 1; t++)
			ans += (t <= (N - fib[K] * t) / fib[K + 1] && (N - fib[K] * t) % fib[K + 1] == 0);
		cout << ans << endl;
	}
	return 0;
}