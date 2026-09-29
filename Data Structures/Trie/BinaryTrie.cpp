#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

// Binary trie (multiset) over numbers in [0, 2^B), B <= 62.
// Node count <= 1 + n * B. Every operation is O(B).
template <int B = 30>
struct BinaryTrie
{
    vector<array<int, 2>> ch{{0, 0}}; // Node 0 is the root, child 0 means "no child"
    vector<int> cnt{0};               // Numbers passing through each node

    BinaryTrie(int n = 0)
    {
        ch.reserve(1 + (size_t)n * B);
        cnt.reserve(1 + (size_t)n * B);
    }

    int size() const { return cnt[0]; }

    void insert(ll x, int d = 1)
    {
        int u = 0;
        cnt[0] += d;
        for (int i = B - 1; i >= 0; i--)
        {
            int b = x >> i & 1;
            if (!ch[u][b])
            {
                ch[u][b] = ch.size();
                ch.push_back({0, 0});
                cnt.push_back(0);
            }
            u = ch[u][b];
            cnt[u] += d;
        }
    }

    int count(ll x) const
    {
        int u = 0;
        for (int i = B - 1; i >= 0 && u >= 0; i--)
            u = ch[u][x >> i & 1] ? ch[u][x >> i & 1] : -1;
        return u < 0 ? 0 : cnt[u];
    }

    bool erase(ll x) // Removes one copy, returns false if x is not present
    {
        if (!count(x))
            return false;
        insert(x, -1);
        return true;
    }

    // Returns max(x ^ y) over all y in the trie. Trie must be non-empty.
    ll maxXor(ll x) const
    {
        int u = 0;
        ll res = 0;
        for (int i = B - 1; i >= 0; i--)
        {
            int b = x >> i & 1, v = ch[u][b ^ 1];
            if (v && cnt[v])
                res |= 1LL << i, u = v;
            else
                u = ch[u][b];
        }
        return res;
    }

    // Returns min(x ^ y) over all y in the trie. Trie must be non-empty.
    ll minXor(ll x) const
    {
        int u = 0;
        ll res = 0;
        for (int i = B - 1; i >= 0; i--)
        {
            int b = x >> i & 1, v = ch[u][b];
            if (v && cnt[v])
                u = v;
            else
                res |= 1LL << i, u = ch[u][b ^ 1];
        }
        return res;
    }

    // Returns how many y in the trie have (x ^ y) < k.
    int countLess(ll x, ll k) const
    {
        if (k >> B) // Every x ^ y is below 2^B <= k
            return size();
        int u = 0, res = 0;
        for (int i = B - 1; i >= 0; i--)
        {
            int bx = x >> i & 1, bk = k >> i & 1;
            if (bk)
            {
                if (ch[u][bx])
                    res += cnt[ch[u][bx]]; // XOR bit 0 here, so smaller than k
                u = ch[u][bx ^ 1];
            }
            else
                u = ch[u][bx];
            if (!u)
                return res;
        }
        return res; // x ^ y == k is not counted
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
    }
    return 0;
}