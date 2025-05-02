#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

class MedianTracker
{
private:
	// Max heap for the lower half of numbers
	priority_queue<ll> lower;
	// Min heap for the upper half of numbers
	priority_queue<ll, vector<ll>, greater<ll>> upper;

public:
	void addNumber(ll num)
	{
		// If lower heap is empty or num is smaller than its top, push to lower heap
		if (lower.empty() || num < lower.top())
			lower.push(num);
		else
			upper.push(num);

		// Rebalance heaps
		if (lower.size() > upper.size() + 1)
		{
			upper.push(lower.top());
			lower.pop();
		}
		else if (upper.size() > lower.size())
		{
			lower.push(upper.top());
			upper.pop();
		}
	}

	ll findMedian()
	{
		return lower.top();
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
	cin >> t;
	while (t--)
	{
		ll a, b, c;
		const ll mod = 1e9 + 7;
		cin >> a >> b >> c >> N;
		ll sum{1};
		ll prevMedian = 1;
		MedianTracker medianTracker;
		medianTracker.addNumber(1);
		for (int i{2}; i <= N; i++)
		{
			ll cur = (a * prevMedian + b * i + c) % mod;
			sum += cur;
			medianTracker.addNumber(cur);
			prevMedian = medianTracker.findMedian();
		}
		cout << sum << endl;
	}
	return 0;
}