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
	ll N, Q;
	// cin >> t;
	while (t--)
	{
		cin >> N;
		vector<ll> vc(N);
		for (int i{}; i < N; i++)
			cin >> vc[i], vc[i] &= 1;
		stack<int> st;
		for (int i{}; i < N; i++)
		{
			if (!st.empty() && st.top() == vc[i])
				st.pop();
			else
				st.push(vc[i]);
		}
		if (st.size() <= 1)
			cout << "YES\n";
		else
			cout << "NO\n";
	}
	return 0;
}