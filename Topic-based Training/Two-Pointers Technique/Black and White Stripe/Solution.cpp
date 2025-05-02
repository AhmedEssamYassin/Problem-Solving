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
		string str;
		cin >> str;
		ll ans{LLONG_MAX}, B{};
		deque<char> deq;
		for (int i{}; i < K; i++)
		{
			deq.push_back(str[i]);
			if (str[i] == 'B')
				B++;
		}
		ans = min(ans, K - B);
		for (int i = K; i < N; i++)
		{
			if (deq.front() == 'B')
				B--;
			deq.pop_front();
			deq.push_back(str[i]);
			if (str[i] == 'B')
				B++;
			ans = min(ans, K - B);
		}
		cout << ans << endl;
	}
	return 0;
}