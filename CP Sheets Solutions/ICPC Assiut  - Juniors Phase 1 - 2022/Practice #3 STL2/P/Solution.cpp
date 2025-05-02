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
		vector<pair<ll, ll>> vc;
		priority_queue<ll> maxHeap;
		priority_queue<ll, vector<ll>, greater<>> minHeap;
		for (int i{}; i < N; i++)
		{
			ll L, R;
			cin >> L >> R;
			vc.push_back({L, R});
			maxHeap.push(L);
			minHeap.push(R);
		}
		// We should remove exactly one segment
		ll maxIntersection{};
		for (const auto &[L, R] : vc)
		{
			bool removedMax = false;
			bool removedMin = false;
			if (maxHeap.top() == L)
				maxHeap.pop(), removedMax = true;
			if (minHeap.top() == R)
				minHeap.pop(), removedMin = true;
			maxIntersection = max(maxIntersection, max(0LL, minHeap.top() - maxHeap.top()));
			if (removedMax)
				maxHeap.push(L);
			if (removedMin)
				minHeap.push(R);
		}
		cout << maxIntersection;
	}
	return 0;
}