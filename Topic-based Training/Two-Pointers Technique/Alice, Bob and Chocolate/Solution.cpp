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
	// cin >> t;
	while (t--)
	{
		cin >> N;
		deque<ll> deq(N);
		for (int i{}; i < N; i++)
			cin >> deq[i];
		if (N == 2)
			return cout << "1 1", 0;
		ll Alice{}, Bob{};
		int L = 0;
		int R = N - 1;
		bitset<100001> vis;
		while (!deq.empty())
		{
			ll Front = deq.front();
			ll Back = deq.back();
			if (L == R && !vis[L])
			{
				Alice++;
				break;
			}
			vis[R] = 1;
			if (Front < Back)
				Alice++, Back -= Front, deq.pop_front(), deq.pop_back(), deq.push_back(Back), L++;
			else if (Back < Front)
				Bob++, Front -= Back, deq.pop_back(), deq.pop_front(), deq.push_front(Front), R--;
			else if (deq.size() == 1)
				Bob++, deq.pop_back();
			else
				Alice++, Bob++, deq.pop_back(), deq.pop_front(), L++, R--;
		}
		cout << Alice << " " << Bob;
	}
	return 0;
}