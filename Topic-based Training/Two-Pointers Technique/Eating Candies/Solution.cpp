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
	ll N;
	cin >> t;
	while (t--)
	{
		cin >> N;
		int *weight = new int[N];
		for (int i = 0; i < N; i++)
			cin >> weight[i];

		int ans{};
		int L{}, R{N - 1}; // Index at the candy that is YET to be eaten
		ll Alice{}, Bob{};
		while (L <= R) // They CANNOT eat the same candy
		{
			Alice += weight[L];

			while (Alice > Bob && R > L)
				Bob += weight[R--];

			if (Alice == Bob)
				ans = max<ll>(ans, (L + 1) + (N - (R + 1)));

			L++;
		}
		cout << ans << endl;
	}
	return 0;
}