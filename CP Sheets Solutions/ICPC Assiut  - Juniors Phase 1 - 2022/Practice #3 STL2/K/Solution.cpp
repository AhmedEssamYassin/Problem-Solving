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
	ll N, M, K;
	cin >> t;
	while (t--)
	{
		cin >> N >> M >> K;
		vector<ll> vc;
		priority_queue<ll, vector<ll>, greater<>> prQue;
		ll len{};
		for (int i{}; i < N; i++)
		{
			ll x;
			cin >> x;
			prQue.push(x);
			len++;
			if (len == M)
			{
				vc.push_back(prQue.top());
				prQue.pop();
				len--;
			}
		}
		while (!prQue.empty())
			vc.push_back(prQue.top()), prQue.pop();
		K--; // To be 0-based
		cout << vc[K] << endl;
	}
	return 0;
}