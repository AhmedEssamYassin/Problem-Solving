#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

// Manacher's algorithm to precompute palindrome radii
vector<int> manacherTransform(const string &s)
{
	string t = "#";
	for (char c : s)
	{
		t += c;
		t += "#";
	}
	int n = t.size();
	vector<int> P(n, 0);
	int center = 0, right = 0;
	for (int i = 0; i < n; i++)
	{
		int mirror = 2 * center - i;
		if (i < right)
			P[i] = min(right - i, P[mirror]);
		while (i - 1 - P[i] >= 0 && i + 1 + P[i] < n && t[i - 1 - P[i]] == t[i + 1 + P[i]])
			P[i]++;
		if (i + P[i] > right)
		{
			center = i;
			right = i + P[i];
		}
	}
	return P;
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
	cin >> t;
	while (t--)
	{
		int n;
		cin >> n;
		string str;
		cin >> str;
		vector<ll> vc(n);
		for (ll &x : vc)
			cin >> x;
		vector<int> P = manacherTransform(str);
		auto isPalindrome = [&](int l, int r)
		{
			int L = l * 2 + 1;
			int R = r * 2 + 1;
			int center = (L + R) / 2;
			int length = R - L;
			return P[center] >= (length / 2);
		};

		int mxLen{1};
		for (int i{}; i < n; i++)
		{
			ll OR = vc[i];
			int l = i, r = i + 1;
			while ((OR & vc[r]) == 0 && r < n)
			{
				OR |= vc[r];
				if (isPalindrome(l, r))
					mxLen = max(mxLen, r - l + 1);
				r++;
			}
		}
		cout << mxLen << endl;
	}
	return 0;
}
