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
		deque<ll> deq;
		for (int i{}; i < N; i++)
		{
			ll ID, type;
			cin >> type >> ID;
			if (type == 1)
				deq.push_back(ID);
			else
			{
				if (ID == deq.front())
					cout << "Yes\n";
				else
					cout << "No\n";
				deq.pop_front();
			}
		}
	}
	return 0;
}