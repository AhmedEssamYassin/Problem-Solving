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

	int Password;
	while (cin >> Password)
	{
		if (Password == 1999)
			return cout << "Correct" << endl, 0;
		else
			cout << "Wrong" << endl;
	}
}