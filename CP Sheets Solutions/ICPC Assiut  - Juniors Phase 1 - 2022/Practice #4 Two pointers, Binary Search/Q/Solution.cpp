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
	vector<ll> triNumbers{1};
	for (ll i = 2; i * i <= 1e12; i++)
		triNumbers.push_back(triNumbers.back() + i);
	cin >> t;
	while (t--)
	{
		ll L, R;
		cin >> L >> R;
		// A triangular number is a number on the form: N * (N + 1)/2 such as: 1, 3, 6, 10 ...
		// A + B = K, K is in range [L, R] and A, B are triangular numbers
		ll cnt{};
		for (int i{}; i < triNumbers.size(); i++)
		{
			if (triNumbers[i] >= R)
				break;
			ll end = upper_bound(triNumbers.begin() + i, triNumbers.end(), R - triNumbers[i]) - triNumbers.begin();
			ll start = lower_bound(triNumbers.begin() + i, triNumbers.end(), L - triNumbers[i]) - triNumbers.begin();
			cnt += (end - start);
		}
		cout << cnt << endl;
	}
	return 0;
}