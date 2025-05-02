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
	ll N, Q;
	// cin >> t;
	while (t--)
	{
		cin >> N;
		vector<ll> vc(N);
		for (int i{}; i < N; i++)
			cin >> vc[i];
		ll L{1}, R{2}, len{2}, maxLen = 2;
		while (R < N)
		{
			if (vc[R] == vc[L] + vc[L - 1])
				len++, R++, L++;
			else
			{
				maxLen = max(maxLen, len);
				len = 2;
				L = R;
				R++;
			}
		}
		maxLen = max(maxLen, len);

		cout << (N < 2 ? 1 : maxLen);
	}
	return 0;
}