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
		map<string, ll> mp; // {name, occurrence}
		for (int i{}; i < N; i++)
		{
			string name;
			cin >> name;
			if (mp.count(name) == 0)
				cout << "OK\n";
			else
				cout << name << mp[name] << endl;
			mp[name]++;
		}
	}
	return 0;
}