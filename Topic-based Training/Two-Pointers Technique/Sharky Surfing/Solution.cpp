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
	ll N, M, L;
	cin >> t;
	while (t--)
	{
		cin >> N >> M >> L;
		vector<pair<int, int>> hurdles(N);
		for (auto &[Li, Ri] : hurdles)
			cin >> Li >> Ri;

		vector<int> X(M), V(M);
		for (int i{}; i < M; i++)
			cin >> X[i] >> V[i];

		bool Possible = true;
		int i = 0;
		ll k = 1, cnt{};
		priority_queue<int> powerUps;
		for (const auto &[Li, Ri] : hurdles)
		{
			while (i < M && X[i] < Li)
				powerUps.push(V[i]), i++;
			while (!powerUps.empty())
			{
				if (k < Ri - Li + 2)
				{
					cnt++;
					k += powerUps.top();
					powerUps.pop();
				}
				else
					break;
			}

			if (k < Ri - Li + 2)
			{
				Possible = false;
				break;
			}
		}

		if (not Possible)
			cout << -1 << endl;
		else
			cout << cnt << endl;
	}
	return 0;
}