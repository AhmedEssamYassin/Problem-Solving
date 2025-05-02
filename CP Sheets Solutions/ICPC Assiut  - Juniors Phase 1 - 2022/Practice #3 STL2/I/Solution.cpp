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
	// cin >> t;
	while (t--)
	{
		cin >> N;
		vector<ll> vc(N);
		map<ll, ll> mp; // {vc[i], index}
		for (int i{}; i < N; i++)
			cin >> vc[i], mp[vc[i]] = i;

		map<ll, bool> hasPair;
		for (int i{}; i < N; i++)
		{
			// vc[i] + vc[j] = powerOf2
			bool found = false;
			for (int j{}; j < 31; j++)
			{
				ll powerOf2 = (1LL << j);
				ll vcJ = powerOf2 - vc[i];
				found |= (mp.count(vcJ) && mp[vcJ] != i);
			}
			if (found)
				hasPair[vc[i]] = true;
		}

		ll cntGood{};
		for (int i{}; i < N; i++)
			cntGood += (hasPair[vc[i]]);
		cout << N - cntGood;
	}
	return 0;
}