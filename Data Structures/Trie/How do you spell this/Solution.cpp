#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct Trie
{
	struct Node
	{
		map<char, Node *> character;
		ll prefix, isEnd; // To count prefixes and strings ending at each node
		ll remaining;	  // To count remaining characters to form a complete existing word
		Node()
		{
			prefix = 0;
			isEnd = 0;
			remaining = LLONG_MAX;
		}
	};

	Node *root;
	Trie() { root = new Node(); }

	void insert(const string &str)
	{
		Node *cur = root;
		ll curLen = str.length();
		cur->remaining = min(cur->remaining, curLen);
		for (const char &C : str)
		{
			if (cur->character[C] == nullptr)
				cur->character[C] = new Node();

			cur = cur->character[C];
			cur->prefix++;
			cur->remaining = min(cur->remaining, --curLen);
		}
		cur->remaining = min(cur->remaining, 0LL); // End of a word
		cur->isEnd++;
	}

	// Can return a boolean, or the actual number of string having `str` as a prefix
	ll checkPrefix(const string &str)
	{
		Node *cur = root;
		for (const char &C : str)
		{
			if (cur->character[C] == nullptr)
				return -1;
			cur = cur->character[C];
		}
		return cur->remaining;
	}

	~Trie() = default;
	// Don't clean unless you need this memory because this makes it much slower
	void clean()
	{
		stack<Node *> stk;
		stk.push(root);
		while (!stk.empty())
		{
			Node *node = stk.top();
			stk.pop();
			for (auto &[_, child] : node->character)
			{
				if (child)
					stk.push(child);
			}
			delete node;
		}
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
		Trie trie;
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