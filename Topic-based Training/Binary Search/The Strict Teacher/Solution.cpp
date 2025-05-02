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
	ll N, m, q, x;
	cin >> t;
	while (t--)
	{
		cin >> N >> m >> q;
		set<ll> st;
		for (int i{}; i < m; i++)
			cin >> x, st.insert(x);

		while (q--)
		{
			ll a;
			cin >> a;
			// Nearest teacher after me (go to left)
			// Nearest teacher before me (go to right)
			// Nearest teacher before and after me (have three choices)
			// max({stay, (before - me)/2, (after - me)/2})
			auto prevIt = st.lower_bound(a);
			// (This case is guaranteed not to occur)
			if (*prevIt == a) // There is a teacher in that cell already, No escape,
			{
				cout << 0 << endl;
				continue;
			}

			if (prevIt != st.end()) // There is a one after me
			{
				if (prevIt == st.begin()) // No one before me
					cout << a - 1 + *prevIt - a << endl;
				else // There is also before me
				{
					ll next = *prevIt;
					ll prev = *(--prevIt);
					if (a - prev < next - a)
					{
						next -= (a - prev - 1);
						cout << (a - prev - 1) + (next - a + 1) / 2 << endl;
					}
					else
					{
						prev += (next - a - 1);
						cout << (next - a - 1) + (a - prev + 1) / 2 << endl;
					}
				}
			}
			else if (st.size() > 0) // There is only before me
			{
				auto it = st.end();
				--it;
				cout << N - a + a - *it << endl;
			}
		}
	}
	return 0;
}