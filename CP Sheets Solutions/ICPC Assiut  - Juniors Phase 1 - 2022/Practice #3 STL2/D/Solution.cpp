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
	ll N, M;
	// cin >> t;
	while (t--)
	{
		cin >> N >> M;
		vector<ll> a(N), b(M);
		for (int i{}; i < N; i++)
			cin >> a[i];
		for (int i{}; i < M; i++)
			cin >> b[i];
		// We will search in b[], let's sort it
		sort(b.begin(), b.end());
		ll cnt{};
		for (const ll &CD : a)
			cnt += (binary_search(b.begin(), b.end(), CD));
		cout << cnt;
	}
	return 0;
}