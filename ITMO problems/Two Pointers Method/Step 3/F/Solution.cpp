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
		int n, m;
		string s, cards;
		cin >> n >> m >> s >> cards;
		int avail[26]{}, used[26]{};
		for (const char &ch : cards)
			avail[ch - 'a']++;

		ll ans = 0, L = 0;
		for (int R{}; R < n; R++)
		{
			int c = s[R] - 'a';
			used[c]++;
			while (used[c] > avail[c])
				used[s[L++] - 'a']--;
			ans += (R - L + 1);
		}
		cout << ans;
	}
	return 0;
}
