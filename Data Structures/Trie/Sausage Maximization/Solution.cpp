#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

template <int B = 40>
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
        ll n;
        cin >> n;
        vector<ll> a(n);
        BinaryTrie binTrie;
        for (int i{}; i < n; i++)
            cin >> a[i];

        ll suff{};
        for (int i = n - 1; i >= 0; i--)
        {
            suff ^= a[i];
            binTrie.insert(suff);
        }

        ll mx = 0, pref{};
        for (int i = 0; i < n; i++)
        {
            mx = max(binTrie.maxXor(pref), mx);
            pref ^= a[i];
        }
        mx = max(binTrie.maxXor(pref), mx);

        cout << mx << endl;
    }
    return 0;
}