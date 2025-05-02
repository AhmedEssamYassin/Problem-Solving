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
	ll N;
	// cin >> t;
	while (t--)
	{
		cin >> N;
		string str;
		cin >> str;
		set<char> st(str.begin(), str.end());
		auto hasAllPokemons = [&](map<char, ll> &freq)
		{
			for (const char &pokemon : st)
			{
				if (freq[pokemon] == 0)
					return false;
			}
			return true;
		};
		ll L{}, R{}, len{}, minLen = N;
		map<char, ll> freq;
		while (R < N)
		{
			freq[str[R]]++; // Expanding
			len++;
			while (hasAllPokemons(freq) && L <= R) // Shrinking
			{
				minLen = min(minLen, len);
				freq[str[L]]--;
				L++;
				len--;
			}
			R++;
		}
		cout << minLen;
	}
	return 0;
}