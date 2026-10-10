#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

// atcoder 419D
int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif
	int t = 1;
	// cin >> t;
	while (t--)
	{
		int n, q;
		cin >> n >> q;
		string S, T;
		cin >> S >> T;
		vector<int> diff(n + 2, 0);
		while (q--)
		{
			int l, r;
			cin >> l >> r;
			--l, --r;
			diff[l] ^= 1;
			diff[r + 1] ^= 1;
		}
		for (int i = 1; i < n; i++)
			diff[i] ^= diff[i - 1];
		for (int i{}; i < n; i++)
		{
			if (diff[i] & 1)
				cout << T[i];
			else
				cout << S[i];
		}
	}
	return 0;
}
