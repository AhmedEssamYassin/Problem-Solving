#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

const int BITS = 30; // change 30 based on number of bits in maximum number
struct BinaryTrie
{
    struct Node
    {
        Node *child[2];
        int freq[2];
        Node()
        {
            child[0] = child[1] = nullptr;
            freq[0] = freq[1] = 0;
        }
    };
    Node *root = new Node();

    BinaryTrie() {}
    void insert(ll x)
    {
        Node *cur = root;
        for (int i = BITS; i >= 0; i--)
        {
            int idx = ((x >> i) & 1); // same as (x & (1 << i)), but this avoids any overflow
            if (cur->child[idx] == 0)
                cur->child[idx] = new Node();
            cur->freq[idx]++;
            cur = cur->child[idx];
        }
    }

    // Recursively erases the number `x` from the binary trie starting at bit index `i`, using the current node `cur`.
    // Decrements the frequency count of the path corresponding to the bits of `x`.
    // If any child node's frequency becomes zero after deletion, it deallocates that node to save memory.
    void erase(ll x, int i, Node *cur)
    {
        if (i == -1)
            return;
        int idx = ((x >> i) & 1);
        erase(x, i - 1, cur->child[idx]);
        cur->freq[idx]--;
        if (cur->freq[idx] == 0)
        {
            delete cur->child[idx];
            cur->child[idx] = NULL;
        }
    }

    // Returns the maximum XOR value obtainable by XORing input `x` with a number already inserted in the binary trie.
    // Along with the maximum XOR value, it also returns the number from the trie that gives this XOR.
    pair<ll, ll> maxXor(ll x)
    {
        Node *cur = root;
        ll retXor = 0, number = 0;
        for (int i = BITS; i >= 0; i--)
        {
            ll idx = ((x >> i) & 1);
            if (cur->child[idx ^ 1])
            {
                retXor |= (1LL << i);
                number |= ((idx ^ 1LL) << i);
                cur = cur->child[idx ^ 1];
            }
            else
            {
                number |= (idx << i);
                cur = cur->child[idx];
            }
        }
        return {retXor, number};
    }
};

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
    int t = 1;
    ll N, Q;
    // cin >> t;
    while (t--)
    {
    }
    return 0;
}