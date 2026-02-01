#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct Trie
{
    struct Node
    {
        Node *character[26];
        int prefix, isEnd; // To count prefixes and strings ending at each node
        Node()
        {
            prefix = 0;
            isEnd = 0;
            for (int i{}; i < 26; i++)
                character[i] = nullptr;
        }
    };

    Node *root;
    Trie() { root = new Node(); }

    void insert(const string &str)
    {
        Node *cur = root;
        for (const char &C : str)
        {
            int idx = (C - 'a'); // To be 0-based index
            if (cur->character[idx] == nullptr)
                cur->character[idx] = new Node();

            cur = cur->character[idx];
            cur->prefix++;
        }
        cur->isEnd++;
    }

    // Can also return a boolean, or even the actual number of string having `str` as a prefix
    ll checkPrefix(const string &str)
    {
        Node *cur = root;
        for (const char &C : str)
        {
            int idx = (C - 'a');
            if (cur->character[idx] == nullptr)
                return false;
            cur = cur->character[idx];
        }
        return cur->prefix;
    }

    // Recursive function to delete a word from given Trie (Assuming it's been inserted before)
    void erase(const string &str)
    {
        Node *cur = root;
        for (const char &C : str)
        {
            int idx = (C - 'a');
            cur = cur->character[idx];
            cur->prefix--;
        }
        cur->isEnd--;
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
            for (int i = 0; i < 26; ++i)
            {
                if (node->character[i])
                    stk.push(node->character[i]);
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
    ll N, Q;
    string str;
    // cin >> t;
    while (t--)
    {
        cin >> N >> Q;
        Trie trie;
        for (int i{}; i < N; i++)
        {
            cin >> str;
            trie.insert(str);
        }
        while (Q--)
        {
            cin >> str;
            cout << trie.checkPrefix(str) << endl;
        }
    }
    return 0;
}