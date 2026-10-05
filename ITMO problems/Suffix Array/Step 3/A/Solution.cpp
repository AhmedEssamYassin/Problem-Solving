#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct SuffixArray
{
	int n;
	string s;
	vector<int> sa, rnk, lcp;
	vector<vector<int>> st; // Sparse table over lcp, built by initLCPQueries()

	SuffixArray(const string &str) : n(str.size()), s(str)
	{
		int m = n + 1; // Suffix n is the empty suffix, smaller than all others
		vector<int> c(m), nc(m), tmp(m), cnt(max(m, 257));
		sa.resize(m);
		for (int i = 0; i < n; i++)
			c[i] = (unsigned char)s[i] + 1;
		for (int i = 0; i < m; i++)
			cnt[c[i]]++;
		for (int i = 1; i < 257; i++)
			cnt[i] += cnt[i - 1];
		for (int i = m - 1; i >= 0; i--)
			sa[--cnt[c[i]]] = i;

		int classes = 1;
		nc[sa[0]] = 0;
		for (int i = 1; i < m; i++)
			nc[sa[i]] = c[sa[i]] == c[sa[i - 1]] ? classes - 1 : classes++;
		swap(c, nc);

		for (int k = 1; classes < m; k <<= 1) // Sort cyclic shifts of length 2k by (c[i], c[i + k])
		{
			for (int i = 0; i < m; i++)
				tmp[i] = (sa[i] - k + m) % m; // Already sorted by the second half
			fill(cnt.begin(), cnt.begin() + classes, 0);
			for (int i = 0; i < m; i++)
				cnt[c[i]]++;
			for (int i = 1; i < classes; i++)
				cnt[i] += cnt[i - 1];
			for (int i = m - 1; i >= 0; i--)
				sa[--cnt[c[tmp[i]]]] = tmp[i];

			classes = 1;
			nc[sa[0]] = 0;
			for (int i = 1; i < m; i++)
			{
				int a = sa[i - 1], b = sa[i];
				bool same = c[a] == c[b] && c[(a + k) % m] == c[(b + k) % m];
				nc[b] = same ? classes - 1 : classes++;
			}
			swap(c, nc);
		}
		sa.erase(sa.begin()); // Drop the empty suffix

		rnk.resize(n);
		for (int i = 0; i < n; i++)
			rnk[sa[i]] = i;

		lcp.assign(max(n - 1, 0), 0); // Kasai
		for (int i = 0, k = 0; i < n; i++)
		{
			if (rnk[i] == n - 1)
			{
				k = 0;
				continue;
			}
			int j = sa[rnk[i] + 1];
			while (i + k < n && j + k < n && s[i + k] == s[j + k])
				k++;
			lcp[rnk[i]] = k;
			if (k)
				k--;
		}
	}

	void initLCPQueries() // O(n log n), needed by queryLCP and compare
	{
		st = {lcp};
		for (int k = 1; (1 << k) <= (int)lcp.size(); k++)
		{
			st.emplace_back(lcp.size() - (1 << k) + 1);
			for (int i = 0; i < (int)st[k].size(); i++)
				st[k][i] = min(st[k - 1][i], st[k - 1][i + (1 << (k - 1))]);
		}
	}

	int queryLCP(int i, int j) const // LCP of suffixes starting at i and j. O(1)
	{
		if (i >= n || j >= n)
			return 0; // Empty suffix
		if (i == j)
			return n - i;
		int a = rnk[i], b = rnk[j];
		if (a > b)
			swap(a, b);
		int k = __lg(b - a);
		return min(st[k][a], st[k][b - (1 << k)]);
	}

	// Compares s[l1..r1] with s[l2..r2]: < 0, 0 or > 0 like strcmp. O(1)
	int compare(int l1, int r1, int l2, int r2) const
	{
		int len1 = r1 - l1 + 1, len2 = r2 - l2 + 1;
		if (min(len1, len2) == 0) // An empty range is smaller than any nonempty one
			return (len1 > len2) - (len1 < len2);
		int L = min(queryLCP(l1, l2), min(len1, len2));
		if (L == min(len1, len2))
			return (len1 > len2) - (len1 < len2);
		return (unsigned char)s[l1 + L] < (unsigned char)s[l2 + L] ? -1 : 1;
	}
};

bool findSubstring(const SuffixArray &suffArr, const string &str, const string &subStr)
{
	// A substring is a prefix of some suffix
	int m = subStr.size();
	const auto &sa = suffArr.sa;
	auto it = partition_point(sa.begin(), sa.end(), [&](int pos)
	                          { return str.compare(pos, m, subStr) < 0; });
	return it != sa.end() && str.compare(*it, m, subStr) == 0;
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
	ll N;
	// cin >> t;
	while (t--)
	{
		string str;
		cin >> str;
		SuffixArray suffArr(str);
		cin >> N;
		while (N--)
		{
			string s;
			cin >> s;
			if (findSubstring(suffArr, str, s))
				cout << "Yes\n";
			else
				cout << "No\n";
		}
	}
	return 0;
}
