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
		deque<ll> deq(N);
		priority_queue<ll> prQue;
		for (int i{}; i < N; i++)
			cin >> deq[i];

		ll Q;
		cin >> Q;
		while (Q--)
		{
			char type;
			cin >> type;
			if (type == 'L')
			{
				if (!deq.empty())
				{
					prQue.push(deq.front());
					deq.pop_front();
				}
			}
			else if (type == 'R')
			{
				if (!deq.empty())
				{
					prQue.push(deq.back());
					deq.pop_back();
				}
			}
			else // 'Q'
			{
				if (!prQue.empty())
				{
					cout << prQue.top() << endl;
					prQue.pop();
				}
				else
					cout << "-1\n";
			}
		}
	}
	return 0;
}