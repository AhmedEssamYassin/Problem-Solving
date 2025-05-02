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
	ll N, K;
	cin >> t;
	while (t--)
	{
		cin >> N >> K;
		vector<ll> vc(N), m(K);
		for (int i{}; i < N; i++)
			cin >> vc[i];

		for (int i{}; i < K; i++)
			cin >> m[i];
		sort(vc.begin(), vc.end());
		deque<ll> deq(vc.begin(), vc.end());

		auto takeNumbers = [&](ll cnt)
		{
			while (cnt > 0 && !deq.empty())
			{
				deq.pop_front();
				cnt--;
			}
		};

		ll sum{};
		auto pred = [&](ll a, ll b)
		{
			if (a == 1 && b != 1)
				return true; // Always place 1 first
			if (b == 1 && a != 1)
				return false; // Push non-1s after 1s
			return a > b;	  // Sort all other numbers in descending order
		};
		sort(m.begin(), m.end(), pred);
		for (int i{}; i < K; i++)
		{
			ll cnt = m[i];
			ll curMax = deq.back();
			sum += curMax;
			deq.pop_back();
			cnt--;
			if (cnt > 0)
			{
				sum += deq.front();
				deq.pop_front();
				cnt--;
			}
			else
				sum += curMax;
			takeNumbers(cnt);
		}
		cout << sum << endl;
	}
	return 0;
}