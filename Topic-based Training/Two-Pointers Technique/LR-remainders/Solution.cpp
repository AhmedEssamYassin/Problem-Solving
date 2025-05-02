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
	ll N, m;
	string cmd;
	cin >> t;
	while (t--)
	{
		cin >> N >> m;
		vector<int> a(N), ans(N, 0);
		for (int i{}; i < N; i++)
			cin >> a[i];

		cin >> cmd;
		int cntL = count(cmd.begin(), cmd.end(), 'L');
		int head, tail;
		head = cntL, tail = cntL - 1;
		ll prod{1};
		int len = N;
		reverse(cmd.begin(), cmd.end());
		for (int i{}; i < N; i++)
		{
			if (cmd[i] == 'L')
				prod = (prod % m * a[--head] % m) % m;
			else
				prod = (prod % m * a[++tail] % m) % m;
			ans[len - i - 1] = prod;
		}
		for (int i = 0; i < len; i++)
			cout << ans[i] << " \n"[i == len - 1];
	}
	return 0;
}