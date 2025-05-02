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
		vector<ll> vc(N);
		for (int i{}; i < N; i++)
			cin >> vc[i];
		stack<ll> sideStreet;
		ll expected = 1;

		for (ll &truck : vc)
		{
			while (!sideStreet.empty() && sideStreet.top() == expected)
			{
				sideStreet.pop();
				expected++;
			}

			if (truck == expected)
				expected++;
			else
				sideStreet.push(truck);
		}

		while (!sideStreet.empty() && sideStreet.top() == expected)
		{
			sideStreet.pop();
			expected++;
		}

		cout << (sideStreet.empty() ? "YES" : "NO") << endl;
	}
	return 0;
}