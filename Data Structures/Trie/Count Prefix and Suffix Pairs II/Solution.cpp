#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

template <int A = 26, char base = 'a'>
struct Trie
{
	vector<array<int, A>> ch{{}};
	vector<int> pref{0}, end{0}; // pref[u] = strings passing through u, end[u] = strings ending at u

	Trie(int totalLen = 0)
	{
		ch.reserve(totalLen + 1);
		pref.reserve(totalLen + 1);
		end.reserve(totalLen + 1);
	}

	void insert(const string &s, int d = 1)
	{
		int u = 0;
		pref[0] += d;
		for (const char &c : s)
		{
			int b = c - base;
			if (!ch[u][b])
			{
				ch[u][b] = ch.size();
				ch.push_back({});
				pref.push_back(0);
				end.push_back(0);
			}
			u = ch[u][b];
			pref[u] += d;
		}
		end[u] += d;
	}

	int find(const string &s) const // Node of s, or -1
	{
		int u = 0;
		for (const char &c : s)
			if (!(u = ch[u][c - base]))
				return -1;
		return u;
	}

	int countPrefix(const string &s) const // Strings having s as a prefix
	{
		int u = find(s);
		return u < 0 ? 0 : pref[u];
	}

	int count(const string &s) const // Copies of s
	{
		int u = find(s);
		return u < 0 ? 0 : end[u];
	}

	int countPrefixesOf(const string &s) const // Inserted strings that are a prefix of s
	{
		int u = 0, res = end[0];
		for (const char &c : s)
		{
			if (!(u = ch[u][c - base]))
				break;
			res += end[u];
		}
		return res;
	}

	bool erase(const string &s) // Removes one copy, false if s is not present
	{
		if (!count(s))
			return false;
		insert(s, -1);
		return true;
	}
};
class Solution
{
public:
	long long countPrefixSuffixPairs(vector<string> &words)
	{
		Trie trie(words.size());
		ll ans = 0;
		for (const string &w : words)
		{
			string key;
			for (int k = 0, m = w.size(); k < m; k++)
				key += w[k], key += w[m - 1 - k];
			ans += trie.countPrefixesOf(key);
			trie.insert(key);
		}
		return ans;
	}
};