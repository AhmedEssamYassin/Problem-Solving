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
		ll minVal = LLONG_MAX;
		ll maxVal = LLONG_MIN;
		for (int i{}; i < N; i++)
		{
			cin >> vc[i];
			minVal = min(minVal, vc[i]);
			maxVal = max(maxVal, vc[i]);
		}

		vector<ll> maxPos, minPos;
		for (int i{}; i < N; i++)
		{
			if (vc[i] == minVal)
				minPos.push_back(i);
			if (vc[i] == maxVal)
				maxPos.push_back(i);
		}

		// Two pointer approach - both arrays are already sorted since we collected indices in order
		int i = 0, j = 0;
		ll minDist = LLONG_MAX;
		while (i < minPos.size() && j < maxPos.size())
		{
			minDist = min(minDist, abs(minPos[i] - maxPos[j]));
			if (minPos[i] < maxPos[j])
				i++;
			else
				j++;
		}

		cout << minDist;
	}
	return 0;
}