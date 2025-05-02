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
	ll N, K, M;
	// cin >> t;
	while (t--)
	{
		cin >> N >> K >> M;
		vector<string> vc(N);
		map<string, ll> cost;
		for (int i{}; i < N; i++)
			cin >> vc[i];
		for (int i{}; i < N; i++)
		{
			ll c;
			cin >> c;
			cost[vc[i]] = c;
		}

		for (int i{}; i < K; i++)
		{
			ll x;
			cin >> x;
			vector<int> positions(x);
			for (int j{}; j < x; j++)
				cin >> positions[j];

			ll minCost = LLONG_MAX;
			for (int j{}; j < x; j++)
				minCost = min(minCost, cost[vc[positions[j] - 1]]);
			// Giving all the words in this group the same minimum cost as they have the same meaning
			for (int j{}; j < x; j++)
				cost[vc[positions[j] - 1]] = minCost;
		}
		ll totalCost{};
		for (int i{}; i < M; i++)
		{
			string word;
			cin >> word;
			totalCost += cost[word];
		}
		cout << totalCost;
	}
	return 0;
}