#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

bool isOk(const vector<ll> &need, const vector<ll> &have, ll K, ll numOfCakes)
{
	ll magicPowder = K;
	ll N = need.size();
	for (int i = 0; i < N; i++)
	{
		if (numOfCakes * need[i] > have[i])
			magicPowder -= numOfCakes * need[i] - have[i];
		if (magicPowder < 0) // we don't have enough of magicPowder to make this numOfCakes
			return false;
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
	ll N, K;
	// cin >> t;
	while (t--)
	{
		cin >> N >> K;
		vector<ll> need(N), have(N);
		for (int i = 0; i < N; i++)
			cin >> need[i];

		for (int i = 0; i < N; i++)
			cin >> have[i];

		ll res{}; // To print 0 if we can't make any cakes
		ll L{}, R = 2000000000LL, mid;
		while (L <= R)
		{
			mid = ((L + R) >> 1);
			/*
			If mid is OK, all before mid is also OK.
			So we set our result initially = mid then move the left pointer (mid + 1) hoping to find a larger
			number of cakes to be OK too otherwise, result still = mid and so on
			*/
			if (isOk(need, have, K, mid))
			{
				res = mid;
				L = mid + 1;
			}
			else
				// If mid is NOT OK, all after mid is NOT OK either. So we move the right pointer to (mid - 1) and continue searching in the remaining space
				R = mid - 1;
		}
		cout << res;
	}
	return 0;
}