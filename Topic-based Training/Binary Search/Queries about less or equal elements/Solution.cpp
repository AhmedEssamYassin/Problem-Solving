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
		vector<int> a(N), b(M);
		for (int i{}; i < N; i++)
			cin >> a[i];

		for (int i{}; i < M; i++)
			cin >> b[i];
		sort(a.begin(), a.end());
		for (int i{}; i < M; i++)
		{
			int idx = upper_bound(a.begin(), a.end(), b[i]) - a.begin();
			cout << idx << " ";
		}
	}
	return 0;
}