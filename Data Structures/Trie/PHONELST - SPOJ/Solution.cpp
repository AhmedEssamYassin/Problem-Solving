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
	cin >> t;
	while (t--)
	{
		ll N;
		cin >> N;
		vector<string> vc(N);
		Trie<10, '0'> trie;
		for (int i{}; i < N; i++)
		{
			cin >> vc[i];
			trie.insert(vc[i]);
		}
		bool check = true;
		for (int i{}; i < N; i++)
			check &= (trie.countPrefix(vc[i]) == 1);
		if (check)
			cout << "YES\n";
		else
			cout << "NO\n";
	}
	return 0;
}