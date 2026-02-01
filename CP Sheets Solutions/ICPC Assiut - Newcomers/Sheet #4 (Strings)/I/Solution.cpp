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
	int N;
	// cin >> t;
	while (t--)
	{
		string S, T;
		cin >> S;
		T = S;
		reverse(S.begin(), S.end());
		if (T == S)
			cout << "YES";
		else
			cout << "NO";
	}
	return 0;
}