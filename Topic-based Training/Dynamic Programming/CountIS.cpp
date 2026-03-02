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
    ll value;
    // Constructors
    Node() { value = 0; }
    Node(ll x) : value(x) {}
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
        Node res = add64(leftNode.value, rightNode.value);
        return res;
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
    void update(int left, int right, int node, int idx, const ll &x)
    {
        if (left == right)
        {
            seg[node].value = add64(seg[node].value, x);
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
    void update(int idx, const ll &x)
    {
        update(0, size - 1, 0, idx, x);
    }
    ll query(int left, int right)
    {
        Node ans = query(0, size - 1, 0, left, right);
        return ans.value;
    }

#undef L
#undef R
#undef mid
};

int countIS(vector<ll> const &vc)
{
    int n = vc.size();
    if (n == 0)
        return 1; // Empty subsequence

    vector<ll> temp = vc;
    CompressedRange cr(temp);
    SegmentTree seg(temp.size());

    ll res = 0; // 1 if we count the empty subsequence
    for (int i = 0; i < n; i++)
    {
        int pos = cr.index(vc[i]);

        // Count of increasing subsequences ending with elements < vc[i]
        ll count = seg.query(0, pos - 1);

        // Each element can either:
        // 1. Start a new subsequence by itself: +1
        // 2. Extend any existing subsequence ending with smaller element: +count
        ll newSubseq = 1 + count;
        res = add64(res, newSubseq);
        seg.update(pos, newSubseq);
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
        cout << countIS(vc);
    }
    return 0;
}
