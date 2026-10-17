#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

template <class T>
T smod(const string &s, T m)
{
	__int128 r = 0;
	for (const char &c : s)
		if (isdigit(c))
			r = (r * 10 + c - '0') % m;
	return (s[0] == '-' && r) ? m - r : (T)r;
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
	// cin >> t;
	while (t--)
	{
		string s;
		cin >> s;
		cout << (s == "0" ? '1' : "6842"[smod<int>(s, 4)]) << endl;
	}
	return 0;
}