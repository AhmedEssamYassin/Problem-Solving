#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

constexpr int BITS = 32;
struct XORBasis
{
    using word = conditional_t<(BITS > 32), uint64_t, uint32_t>;
    array<word, BITS> basis{}; // basis[i] is 0 or has its highest bit at i
    int sz = 0;

    XORBasis(word x = 0) { insertVector(x); }

    bool insertVector(word x) // true if x was independent of the basis
    {
        for (int i = 0; x && sz < BITS; x ^= basis[i])
            if (!basis[i = __lg(x)])
                return basis[i] = x, sz++, true;
        return false;
    }

    bool canRepresent(word x) const
    {
        for (int i = 0; x; x ^= basis[i])
            if (!basis[i = __lg(x)])
                return 0;
        return 1;
    }

    word getMaxXor() const
    {
        word r = 0;
        for (int i = BITS; i--;)
            r = max(r, r ^ basis[i]);
        return r;
    }

    XORBasis &operator+=(const XORBasis &o)
    {
        if (o.sz == BITS) // Full basis spans everything
            return *this = o;
        for (word v : o.basis)
            insertVector(v);
        return *this;
    }
    friend XORBasis operator+(XORBasis a, const XORBasis &b) { return a += b; }

    void clear() { *this = {}; }
};

struct SegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
    struct Node
    {
        ll value;
        XORBasis xb;
        Node() { value = 0; }
        Node(const ll &N) : value(N) { xb.insertVector(N); }
    };
    int size;
    vector<Node> seg;
    Node merge(const Node &leftNode, const Node &rightNode)
    {
        Node res;
        res.value = (leftNode.value ^ rightNode.value);
        res.xb = (leftNode.xb + rightNode.xb);
        return res;
    }
    void build(int left, int right, int node, const vector<ll> &arr)
    {
        // If the segment has only one element, leaf node
        if (left == right)
        {
            if (left < arr.size())
                seg[node] = arr[left];
            return;
        }
        // Recursively build the left child
        build(left, mid, L, arr);
        // Recursively build the right child
        build(mid + 1, right, R, arr);
        // Merge the children values
        seg[node] = merge(seg[L], seg[R]);
    }
    void update(int left, int right, int node, int idx, const ll &val)
    {
        // If the range is invalid, return
        if (left == right)
        {
            seg[node].value ^= val;
            seg[node].xb.clear();
            seg[node].xb.insertVector(seg[node].value);
            return;
        }
        // Recursively update the left child
        if (idx <= mid)
            update(left, mid, L, idx, val);
        // Recursively update the right child
        else
            update(mid + 1, right, R, idx, val);
        // Merge the children values
        seg[node] = merge(seg[L], seg[R]);
    }
    Node query(int left, int right, int node, int leftQuery, int rightQuery)
    {
        // If the range is invalid, return a value that does NOT to affect other queries
        if (left > rightQuery || right < leftQuery)
            return Node();

        // If the range matches the segment
        if (left >= leftQuery && right <= rightQuery)
            return seg[node];

        return merge(query(left, mid, L, leftQuery, rightQuery), query(mid + 1, right, R, leftQuery, rightQuery));
    }

public:
    SegmentTree(const vector<ll> &arr)
    {
        size = 1;
        int n = arr.size();
        while (size < n)
            size <<= 1;
        seg = vector<Node>(2 * size);
        build(0, size - 1, 0, arr);
    }
    void update(int idx, const ll &val)
    {
        update(0, size - 1, 0, idx, val);
    }
    pair<ll, XORBasis> query(int left, int right)
    {
        Node ans = query(0, size - 1, 0, left, right);
        return {ans.value, ans.xb};
    }

#undef L
#undef R
#undef mid
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
        int n, q;
        cin >> n >> q;

        vector<ll> arr(n);
        for (int i = 0; i < n; i++)
            cin >> arr[i];
        vector<ll> d{arr[0]};
        for (int i = 1; i < n; i++)
            d.push_back(arr[i] ^ arr[i - 1]);
        SegmentTree seg(d);

        while (q--)
        {
            int type;
            cin >> type;

            if (type == 1)
            {
                int l, r;
                ll k;
                cin >> l >> r >> k;
                l--;
                r--;
                seg.update(l, k);
                if (r + 1 < n)
                    seg.update(r + 1, k);
            }
            else
            {
                int l, r;
                cin >> l >> r;
                l--;
                r--;
                auto [pref, xb] = seg.query(l + 1, r);
                xb += seg.query(0, l).first;
                cout << (1LL << xb.sz) << endl;
            }
        }
    }
    return 0;
}