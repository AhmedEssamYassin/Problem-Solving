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
		string S, T;
		cin >> S >> T;
		map<ll, vector<int>> group;
		for (int i{}; i < N; i++)
		{
			int id;
			cin >> id, group[id].push_back(i);
		}
		// Every group having the same id can be freely sorted, just check they have the same count of numbers
		bool check = true;
		for (const auto &[id, List] : group)
		{
			vector<ll> freqS(26, 0), freqT(26, 0);
			for (const int &i : List)
			{
				freqS[S[i] - 'a']++;
				freqT[T[i] - 'a']++;
			}
			check &= (freqS == freqT);
		}
		if (check)
			cout << "YES\n";
		else
			cout << "NO\n";
	}
	return 0;
}