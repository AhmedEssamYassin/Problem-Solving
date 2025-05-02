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
	int N, Q;
	// cin >> t;
	while (t--)
	{
		cin >> Q;
		deque<int> deq;
		while (Q--)
		{
			string query;
			cin >> query;
			if (query == "toFront")
			{
				cin >> N;
				deq.push_front(N);
			}
			else if (query == "push_back")
			{
				cin >> N;
				deq.push_back(N);
			}
			else if (query == "front")
			{
				if (deq.empty())
					cout << "No job for Ada?\n";
				else
					cout << deq.front() << endl, deq.pop_front();
			}
			else if (query == "back")
			{
				if (deq.empty())
					cout << "No job for Ada?\n";
				else
					cout << deq.back() << endl, deq.pop_back();
			}
			else if (query == "reverse")
				reverse(deq.begin(), deq.end());
		}
	}
	return 0;
}