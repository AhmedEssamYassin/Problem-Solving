#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct Trie
{
	struct Node
	{
		map<char, Node *> character;
		set<int> idxPref;			 // Indices of strings having a prefix at this node
		unique_ptr<set<int>> idxEnd; // Indices of strings ending at this node, created on first use
	};

	Node *root = new Node();

	Node *child(Node *cur, char C) const
	{
		auto it = cur->character.find(C);
		return it == cur->character.end() ? nullptr : it->second;
	}

	static bool inRange(const set<int> &st, int L, int R) // Some index in [L..R]
	{
		auto it = st.lower_bound(L);
		return it != st.end() && *it <= R;
	}

	Node *find(const string &str) const // Node of str, or nullptr
	{
		Node *cur = root;
		for (const char &C : str)
			if (!(cur = child(cur, C)))
				return nullptr;
		return cur;
	}

	void insert(const string &str, int j)
	{
		Node *cur = root;
		cur->idxPref.insert(cur->idxPref.end(), j);
		for (const char &C : str)
		{
			Node *&nxt = cur->character[C];
			if (!nxt)
				nxt = new Node();
			cur = nxt;
			cur->idxPref.insert(cur->idxPref.end(), j); // O(1) when j is larger than every index here
		}
		if (!cur->idxEnd)
			cur->idxEnd = make_unique<set<int>>();
		cur->idxEnd->insert(cur->idxEnd->end(), j);
	}

	// Is some string with index in [L..R] a prefix of str
	bool searchPrefix(const string &str, int L, int R) const
	{
		Node *cur = root;
		for (size_t k = 0;; k++)
		{
			if (cur->idxEnd && inRange(*cur->idxEnd, L, R))
				return true;
			if (k == str.size() || !(cur = child(cur, str[k])))
				return false;
		}
	}

	// Number of strings having str as a prefix
	ll checkPrefix(const string &str) const
	{
		Node *cur = find(str);
		return cur ? cur->idxPref.size() : 0;
	}

	// Is str a prefix of some string with index in [L..R]
	bool checkPrefix(const string &str, int L, int R) const
	{
		Node *cur = find(str);
		return cur && inRange(cur->idxPref, L, R);
	}

	// Removes string str with index j, false if it is not present
	bool erase(const string &str, int j)
	{
		Node *cur = find(str);
		if (!cur || !cur->idxEnd || !cur->idxEnd->erase(j))
			return false;
		cur = root;
		cur->idxPref.erase(j);
		for (const char &C : str)
		{
			cur = child(cur, C);
			cur->idxPref.erase(j);
		}
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
		cin >> N;
		vector<string> vc(N);
		Trie trie;
		for (int i{}; i < N; i++)
			cin >> vc[i], trie.insert(vc[i], i);
		cin >> Q;
		while (Q--)
		{
			ll type, i, L, R;
			string s;
			cin >> type;
			if (type == 1)
			{
				cin >> i >> s;
				i--;
				trie.erase(vc[i], i);
				trie.insert(s, i);
				vc[i] = s;
			}
			else if (type == 2)
			{
				cin >> L >> R >> s;
				L--, R--;
				if (trie.searchPrefix(s, L, R))
					cout << "Y\n";
				else
					cout << "N\n";
			}
			else
			{
				cin >> L >> R >> s;
				L--, R--;
				if (trie.checkPrefix(s, L, R))
					cout << "Y\n";
				else
					cout << "N\n";
			}
		}
	}
	return 0;
}