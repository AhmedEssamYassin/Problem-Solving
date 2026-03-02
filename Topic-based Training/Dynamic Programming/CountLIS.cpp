#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

const ll mod = 1e9 + 7;

#define double_size_t std::conditional_t<(mod > (1LL << 31)), __int128_t, long long>
inline ll add64(const ll &a, const ll &b)
{
    double_size_t res = double_size_t(a) + b;
    if (res >= mod)
        res -= mod;
    return res;
}

inline ll sub64(const ll &a, const ll &b)
{
    double_size_t res = double_size_t(a) - b;
    if (res < 0)
        res += mod;
    if (res >= mod)
        res -= mod;
    return res;
}

inline ll mult64(const ll &a, const ll &b)
{
    return double_size_t(a) * b % mod;
}

class CompressedRange
{
private:
    vector<ll> init;
    void compress(vector<ll> &vec)
    {
        sort(vec.begin(), vec.end());
        vec.erase(unique(vec.begin(), vec.end()), vec.end());
    }

public:
    CompressedRange(vector<ll> &vec)
    {
        init = vec;
        compress(init);
    }
    int index(ll val)
    {
        return lower_bound(init.begin(), init.end(), val) - init.begin();
    }
    ll initVal(int idx)
    {
        return init[idx];
    }
};

struct Node
{
    ll len;
    ll cnt;
    // Constructors
    Node() { len = 0, cnt = 0; }
    Node(ll l, ll c) : len(l), cnt(c) {}
};

struct SegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
    int size;
    vector<Node> seg;
    Node merge(const Node &leftNode, const Node &rightNode)
    {
        if (leftNode.len > rightNode.len)
            return leftNode;
        if (rightNode.len > leftNode.len)
            return rightNode;
        return Node(leftNode.len, add64(leftNode.cnt, rightNode.cnt));
    }
    void build(int left, int right, int node)
    {
        if (left == right) // Leaf Node (single element)
        {
            seg[node] = Node();
            return;
        }
        // Building left node
        build(left, mid, L);

        // Building right node
        build(mid + 1, right, R);

        // Returning to parent nodes
        seg[node] = merge(seg[L], seg[R]);
    }
    void update(int left, int right, int node, int idx, const Node &x)
    {
        if (left == right)
        {
            seg[node] = merge(seg[node], x);
            return;
        }
        if (idx <= mid)
            update(left, mid, L, idx, x);
        else
            update(mid + 1, right, R, idx, x);
        // Updating while returning to parent nodes
        seg[node] = merge(seg[L], seg[R]);
    }
    Node query(int left, int right, int node, int leftQuery, int rightQuery)
    {
        // Out of range
        if (right < leftQuery || left > rightQuery)
            return Node(); // A value that doesn't affect the query

        // The whole range is the answer
        if (left >= leftQuery && right <= rightQuery)
            return seg[node];

        // ONLY a part of this segment belongs to the query
        Node leftSegment = query(left, mid, L, leftQuery, rightQuery);
        Node rightSegment = query(mid + 1, right, R, leftQuery, rightQuery);
        return merge(leftSegment, rightSegment);
    }

public:
    SegmentTree(int n)
    {
        size = 1;
        while (size < n)
            size <<= 1;
        seg = vector<Node>(2 * size);
        build(0, size - 1, 0);
    }
    void update(int idx, const Node &x)
    {
        update(0, size - 1, 0, idx, x);
    }
    Node query(int left, int right)
    {
        return query(0, size - 1, 0, left, right);
    }

#undef L
#undef R
#undef mid
};

int countLIS(vector<ll> vc)
{
    int n = vc.size();
    if (n == 0)
        return 0;
    for (ll &x : vc)
        x += 1e9 + 1;
    vector<ll> temp = vc; // For CompressedRange constructor
    CompressedRange cr(temp);
    SegmentTree seg(n);

    int maxLen = 0;
    vector<pair<ll, ll>> dp(n);

    for (int i = 0; i < n; i++)
    {
        int pos = cr.index(vc[i]);

        // Query for best LIS for elements < vc[i]
        Node best = ((pos > 0) ? seg.query(0, pos - 1) : Node()); // Empty query returns Node()

        // Current element extends the LIS by 1
        // If no previous elements (best.cnt == 0), this element starts a new LIS
        ll newCnt = (best.cnt == 0) ? 1 : best.cnt;
        dp[i] = {best.len + 1, newCnt};
        maxLen = max(maxLen, (int)dp[i].first);

        // Update segment tree
        seg.update(pos, Node(dp[i].first, dp[i].second));
    }

    // Count all LIS with maximum length
    ll res = 0;
    for (int i = 0; i < n; i++)
    {
        if (dp[i].first == maxLen)
            res = add64(res, dp[i].second);
    }

    return res;
}

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
        vector<ll> vc(n);
        for (int i{}; i < n; i++)
            cin >> vc[i];
        cout << countLIS(vc);
    }
    return 0;
}
