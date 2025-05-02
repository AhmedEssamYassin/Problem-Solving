#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct comparator
{
	inline bool operator()(const pair<ll, ll> &P1, const pair<ll, ll> &P2)
	{
		// Prioritize the greater difference, but the priority queue operator is reversed
		return abs(P1.first - P1.second) < abs(P2.first - P2.second);
	}
};

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
		ll K1, K2;
		cin >> N >> K1 >> K2;
		vector<ll> a(N), b(N);
		priority_queue<pair<ll, ll>, vector<pair<ll, ll>>, comparator> prQue;
		for (int i{}; i < N; i++)
			cin >> a[i];
		for (int i{}; i < N; i++)
			cin >> b[i];
		for (int i{}; i < N; i++)
			prQue.push({a[i], b[i]});

		while (K1 > 0)
		{
			auto [ai, bi] = prQue.top();
			prQue.pop();
			if (ai > bi && K1)
				ai--, K1--;
			else if (K1)
				ai++, K1--;
			// Push {ai, bi} after modification
			prQue.push({ai, bi});
		}
		while (K2 > 0)
		{
			auto [ai, bi] = prQue.top();
			prQue.pop();
			if (bi > ai && K2)
				bi--, K2--;
			else if (K2)
				bi++, K2--;
			// Push {ai, bi} after modification
			prQue.push({ai, bi});
		}
		ll sum{};
		while (!prQue.empty())
		{
			auto [ai, bi] = prQue.top();
			prQue.pop();
			sum += (ai - bi) * (ai - bi);
		}
		cout << sum;
	}
	return 0;
}