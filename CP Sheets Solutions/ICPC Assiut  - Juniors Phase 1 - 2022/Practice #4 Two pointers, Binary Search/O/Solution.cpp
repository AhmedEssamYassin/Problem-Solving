#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

bool isOk(const string &str, ll len, ll k)
{
	bool check = false;
	ll cntA{}, cntB{};
	ll N = str.length();
	for (int i{}; i < len; i++)
	{
		if (str[i] == 'a')
			cntA++;
		else
			cntB++;
	}
	check |= (k >= min(cntA, cntB));
	for (int i = len, j = 0; i < N; i++, j++)
	{
		if (str[i] == 'a')
			cntA++;
		else
			cntB++;

		if (str[j] == 'a')
			cntA--;
		else
			cntB--;
		check |= (k >= min(cntA, cntB));
	}
	return check;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
	int t = 1;
	ll N, k;
	// cin >> t;
	while (t--)
	{
		cin >> N >> k;
		string str;
		cin >> str;
		ll L{}, R = N, ans{};
		while (L <= R)
		{
			ll mid = (L + R) / 2;
			if (isOk(str, mid, k))
				ans = mid, L = mid + 1;
			else
				R = mid - 1;
		}
		cout << ans;
	}
	return 0;
}