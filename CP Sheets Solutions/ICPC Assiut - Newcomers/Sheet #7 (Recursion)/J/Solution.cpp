#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

ll factorial(ll N)
{
	if (N == 0)
		return 1;
	return N * factorial(N - 1);
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif
	int t = 1;
	int N;
	// cin >> t;
	while (t--)
	{
		cin >> N;
		cout << factorial(N);
	}
	return 0;
}