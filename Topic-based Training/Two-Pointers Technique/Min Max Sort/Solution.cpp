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
		vector<int> p(N + 1);
		for (int i{}; i < N; i++)
		{
			int x;
			cin >> x;
			p[x] = i;
		}
		int L = (N + 1) / 2, R = (N + 1) / 2 + !(N & 1);
		while (L >= 1 && R <= N && p[L] < p[L + 1] && p[R] > p[R - 1] || L == R)
		{
			L--;
			R++;
		}
		cout << ((N - (R - L) + 1) / 2) << endl;
	}
	return 0;
}