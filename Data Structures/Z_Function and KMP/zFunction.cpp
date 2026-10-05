#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

// z[i] = LCP of s and s[i..n - 1], z[0] = n. O(n)
template <typename T>
vector<int> zFunction(const T &s)
{
	int n = s.size();
	vector<int> z(n);
	if (n)
		z[0] = n;
	for (int i = 1, l = 0, r = 0; i < n; i++)
	{
		if (i < r)
			z[i] = min(r - i, z[i - l]);
		while (i + z[i] < n && s[z[i]] == s[i + z[i]])
			z[i]++;
		if (i + z[i] > r)
			l = i, r = i + z[i];
	}
	return z;
}

// Start positions p of pat in txt, match is txt[p..p + m - 1]. O(n + m)
vector<int> findOccurrences(const string &txt, const string &pat)
{
	int m = pat.size(), n = txt.size();
	vector<int> pos;
	if (m == 0 || m > n)
		return pos;
	vector<int> z = zFunction(pat + txt);
	for (int i = m; i <= n; i++)
		if (z[i] >= m)
			pos.push_back(i - m);
	return pos;
}

// Smallest p with s[i] == s[i + p]. O(n)
int smallestPeriod(const string &s)
{
	int n = s.size();
	vector<int> z = zFunction(s);
	for (int p = 1; p < n; p++)
		if (p + z[p] == n)
			return p;
	return n;
}

// Shortest t with s = t^k. O(n)
int compressedLength(const string &s)
{
	int n = s.size();
	vector<int> z = zFunction(s);
	for (int p = 1; p < n; p++)
		if (n % p == 0 && p + z[p] == n)
			return p;
	return n;
}

// All border lengths, ascending. O(n)
vector<int> borders(const string &s)
{
	int n = s.size();
	vector<int> z = zFunction(s), res;
	for (int i = n - 1; i >= 1; i--)
		if (i + z[i] == n)
			res.push_back(z[i]);
	return res;
}

// cnt[k] = occurrences of s[0..k - 1] in s. O(n)
vector<int> prefixOccurrences(const string &s)
{
	int n = s.size();
	vector<int> z = zFunction(s), cnt(n + 2, 0);
	for (int i = 0; i < n; i++)
		cnt[z[i]]++;
	for (int k = n - 1; k >= 1; k--)
		cnt[k] += cnt[k + 1];
	return cnt;
}

// Longest border that also occurs strictly inside s. O(n)
int longestMiddleBorder(const string &s)
{
	int n = s.size(), best = 0, mx = 0;
	vector<int> z = zFunction(s);
	for (int i = 1; i < n; i++)
	{
		if (i + z[i] == n && mx >= z[i])
			best = max(best, z[i]);
		mx = max(mx, z[i]);
	}
	return best;
}

// Distinct substrings. O(n^2)
ll distinctSubstrings(const string &s)
{
	ll res = 0;
	string cur;
	for (const char &c : s)
	{
		cur.insert(cur.begin(), c);
		vector<int> z = zFunction(cur);
		int mx = 0;
		for (int i = 1; i < (int)cur.size(); i++)
			mx = max(mx, z[i]);
		res += cur.size() - mx;
	}
	return res;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif
	string txt, pat;
	int len;
	while (cin >> len >> pat >> txt)
	{
		for (const int &p : findOccurrences(txt, pat))
			cout << p << endl;
	}
	return 0;
}