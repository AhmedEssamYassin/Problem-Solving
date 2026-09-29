#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

template <int A = 26, char base = 'a'>
struct Trie
{
	vector<array<int, A>> ch{{}};
	vector<int> pref{0}, end{0}; // pref[u] = strings passing through u, end[u] = strings ending at u
	vector<int> rem{INT_MAX};
	Trie(int totalLen = 0)
	{
		ch.reserve(totalLen + 1);
		pref.reserve(totalLen + 1);
		end.reserve(totalLen + 1);
		rem.reserve(totalLen + 1);
	}

	void insert(const string &s, int d = 1)
	{
		int u = 0, left = s.length();
		pref[0] += d;
		rem[0] = min(rem[0], left);
		for (const char &c : s)
		{
			int b = c - base;
			if (!ch[u][b])
			{
				ch[u][b] = ch.size();
				ch.push_back({});
				pref.push_back(0);
				end.push_back(0);
				rem.push_back(INT_MAX);
			}
			u = ch[u][b];
			pref[u] += d;
			rem[u] = min(rem[u], --left);
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

	int checkPrefix(const string &s) const
	{
		int u = find(s);
		return u < 0 ? -1 : rem[u];
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

	bool erase(const string &s) // Removes one copy, false if s is not present
	{
		if (!count(s))
			return false;
		insert(s, -1);
		return true;
	}
};

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
		ll N, Q;
		cin >> N >> Q;
		Trie trie(N);
		string str;
		for (int i{}; i < N; i++)
		{
			cin >> str;
			trie.insert(str);
		}
		for (int i{}; i < Q; i++)
		{
			cin >> str;
			ll ans = trie.checkPrefix(str);
			cout << ans << endl;
		}
	}
	return 0;
}