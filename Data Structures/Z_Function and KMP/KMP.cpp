#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

// b[i] = longest proper border of p[0..i - 1], b[0] = -1 (sentinel). O(m)
vector<int> preprocessKMP(const string &p)
{
	int m = p.size();
	vector<int> b(m + 1);
	b[0] = -1;
	for (int i = 0, j = -1; i < m;)
	{
		while (j >= 0 && p[i] != p[j])
			j = b[j];
		b[++i] = ++j;
	}
	return b; // pi[n - 1] is b[n]
}

// Start positions p of pat in txt, match is txt[p..p + m - 1]. O(n + m)
vector<int> searchKMP(const string &txt, const string &pat)
{
	int n = txt.size(), m = pat.size();
	vector<int> pos;
	if (m == 0 || m > n)
		return pos;
	vector<int> b = preprocessKMP(pat);
	for (int i = 0, j = 0; i < n;)
	{
		while (j >= 0 && txt[i] != pat[j])
			j = b[j];
		i++, j++;
		if (j == m)
		{
			pos.push_back(i - m);
			j = b[j];
		}
	}
	return pos;
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
	string pat, txt;
	int len;
	// cin >> t;
	while (cin >> len >> pat >> txt)
	{
		vector<int> pos = searchKMP(txt, pat);
		for (const int &x : pos)
			cout << x << endl;
	}
	return 0;
}
