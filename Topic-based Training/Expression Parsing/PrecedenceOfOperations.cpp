#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif
	int t = 1;
	// cin >> t;
	while (t--)
	{
		auto eval = [](const vector<ll> &a, const vector<char> &op) -> ll
		{
			ll sum = 0, term = a[0];
			for (int i = 0; i < (int)op.size(); i++)
			{
				ll v = a[i + 1];
				if (op[i] == '+')
					sum += term, term = v;
				else if (op[i] == '-')
					sum += term, term = -v;
				else if (op[i] == '*')
					term *= v;
				else if (v == 0)
					return LLONG_MAX; // division by zero
				else
					term /= v;
			}
			return sum + term;
		};
	}
	return 0;
}
