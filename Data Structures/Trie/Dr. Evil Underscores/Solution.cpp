#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

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

    // Min over X of max(y ^ X) for y in the subtree of u, using bits i..0
    ll minMaxXor(int u = 0, int i = B - 1) const
    {
        if (i < 0)
            return 0;
        int l = ch[u][0], r = ch[u][1];
        if (!l)
            return minMaxXor(r, i - 1);
        if (!r)
            return minMaxXor(l, i - 1);
        return (1LL << i) + min(minMaxXor(l, i - 1), minMaxXor(r, i - 1));
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
        int n;
        cin >> n;
        BinaryTrie<30> binTrie(n);
        for (int i = 0; i < n; i++)
        {
            ll x;
            cin >> x;
            binTrie.insert(x);
        }
        cout << binTrie.minMaxXor() << endl;
    }
    return 0;
}