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
		set<vector<ll>> unique_sequences;

		// Try all possible swaps
		for (int i = 0; i < N; i++)
		{
			for (int j = i + 1; j < N; j++)
			{
				vector<ll> swapped = vc;
				swap(swapped[i], swapped[j]);	  // Swap elements
				unique_sequences.insert(swapped); // Store in set
			}
		}

		cout << unique_sequences.size() << endl;
	}
	return 0;
}