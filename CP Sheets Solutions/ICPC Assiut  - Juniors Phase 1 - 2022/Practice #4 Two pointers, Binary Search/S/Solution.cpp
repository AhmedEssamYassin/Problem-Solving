#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

bool isOk(vector<ll> pile, ll M, ll Time)
{
	int i = pile.size() - 1;

	while (i >= 0)
	{
		// Skip empty piles
		while (i >= 0 && pile[i] == 0)
			i--;

		// If we've processed all piles, we're done
		if (i < 0)
			break;

		// Check if we have any workers left
		if (M == 0)
			return false;

		// Assign a worker
		M--;
		ll remainingTime = Time;

		// Subtract the time to reach this pile
		remainingTime -= (i + 1);

		// Process piles until worker runs out of time
		while (remainingTime > 0 && i >= 0)
		{
			ll currentPileSize = pile[i];

			if (remainingTime < currentPileSize)
			{
				// Cannot complete this pile
				pile[i] -= remainingTime;
				break;
			}
			else
			{
				// Complete this pile and move to the next
				remainingTime -= currentPileSize;
				i--;
			}
		}
	}

	return true;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
	int t = 1;
	ll N, M;
	// cin >> t;
	while (t--)
	{
		cin >> N >> M;
		vector<ll> pile(N);
		for (int i{}; i < N; i++)
			cin >> pile[i];
		ll L{}, R = (1LL << 60), ans{};
		while (L <= R)
		{
			ll mid = ((L + R) >> 1);
			if (isOk(pile, M, mid))
				ans = mid, R = mid - 1;
			else
				L = mid + 1;
		}
		cout << ans;
	}
	return 0;
}