#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

// Flattening binary trees is useful to do subtree queries.

struct LazySegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
    struct Node
    {
        ll value;
        Node() {}
        Node(const ll &N) : value(N) {}
    };
    struct LazyNode
    {
        ll value;
        LazyNode() {}
        LazyNode(const ll &N) : value(N) {}
        LazyNode operator=(const LazyNode &RHS)
        {
            value = RHS.value;
            return *this;
        }
    };
    int size;
    vector<Node> seg;
    vector<LazyNode> lazy;
    Node merge(const Node &leftNode, const Node &rightNode)
    {
        Node res = (leftNode.value + rightNode.value);
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
    void push(int left, int right, int node)
    {
        // Propagate the value
        if (lazy[node].value == 0)
            return;
        seg[node].value = (right - left + 1) * lazy[node].value;
        // If the node is not a leaf
        if (left != right)
        {
            // Update the lazy values for the left child
            lazy[L] = lazy[node];
            // Update the lazy values for the right child
            lazy[R] = lazy[node];
        }
        // Reset the lazy value
        lazy[node] = 0;
    }
    void update(int left, int right, int node, int leftQuery, int rightQuery, const ll &val)
    {
        push(left, right, node);
        // If the range is invalid, return
        if (left > rightQuery || right < leftQuery)
            return;
        // If the range matches the segment
        if (left >= leftQuery && right <= rightQuery)
        {
            // Update the lazy value
            lazy[node] = val;
            // Apply the update immediately
            push(left, right, node);
            return;
        }
        // Recursively update the left child
        update(left, mid, L, leftQuery, rightQuery, val);
        // Recursively update the right child
        update(mid + 1, right, R, leftQuery, rightQuery, val);
        // Merge the children values
        seg[node] = merge(seg[L], seg[R]);
    }
    Node query(int left, int right, int node, int leftQuery, int rightQuery)
    {
        // Apply the pending updates if any
        push(left, right, node);
        // If the range is invalid, return a value that does NOT to affect other queries
        if (left > rightQuery || right < leftQuery)
            return 0;

        // If the range matches the segment
        if (left >= leftQuery && right <= rightQuery)
            return seg[node];

        return merge(query(left, mid, L, leftQuery, rightQuery), query(mid + 1, right, R, leftQuery, rightQuery));
    }

public:
    LazySegmentTree(const vector<ll> &arr)
    {
        size = 1;
        int n = arr.size();
        while (size < n)
            size <<= 1;
        seg = vector<Node>(2 * size, 0);
        lazy = vector<LazyNode>(2 * size, 0); // Initialize the LazyNode with a Not-possible value
        build(0, size - 1, 0, arr);
    }
    void update(int left, int right, const ll &val)
    {
        update(0, size - 1, 0, left, right, val);
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

template <typename T>
T mult64(const T &a, const T &b, T mod)
{
    return (__int128_t)a * b % mod;
}

template <typename T>
T modPow(T N, T power, T mod)
{
    if (N % mod == 0 || N == 0)
        return 0;
    if (N == 1 || power == 0)
        return 1;
    T res{1};
    while (power)
    {
        if (power & 1)
            res = mult64(res, N, mod);
        N = mult64(N, N, mod);
        power >>= 1;
    }
    return res;
}

template <typename T>
bool isPrime(T N)
{
    constexpr uint64_t MASK = 0x28208A20A08A28ACULL;
    constexpr uint32_t WHEEL30 = 0x208A2882;
    if (N < 64)
        return (MASK >> N) & 1;
    if (!((WHEEL30 >> (uint32_t)(N % 30)) & 1))
        return false;
    T d = N - 1;
    int s{};
    while (!(d & 1))
        d >>= 1, ++s;
    for (const T &a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022})
    {
        T p = modPow(a % N, d, N), i = s;
        while (p != 1 && p != N - 1 && a % N && i--)
            p = mult64(p, p, N);
        if (p != N - 1 && i != s)
            return false;
    }
    return true;
}

bool isSumOf2Primes(ll N)
{
    if (!(N & 1) && N > 2) // GoldBach Conjecture
        return true;
    else
        return isPrime(N - 2);
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
        ll N;
        cin >> N;
        vector<ll> a(N + 1);
        for (int i{1}; i <= N; i++)
            cin >> a[i];
        vector<vector<ll>> Tree(N + 1);
        int anyNode = 1;
        for (int i{}; i < N - 1; i++)
        {
            ll u, v;
            cin >> u >> v;
            anyNode = u;
            Tree[u].push_back(v);
            Tree[v].push_back(u);
        }
        ll root = 1;
        // If the tree is not rooted
        // root = anyNode;
        int currTime = 0;
        vector<ll> in(N + 1), out(N + 1);
        function<void(int, int)> eulerTour = [&](int u, int prev) -> void
        {
            in[u] = currTime++;
            for (const ll &v : Tree[u])
            {
                if (v != prev)
                    eulerTour(v, u);
            }
            out[u] = currTime - 1;
        };
        eulerTour(root, -1);
        vector<ll> nodes(N);
        for (int u = 1; u <= N; u++)
            nodes[in[u]] = a[u];

        LazySegmentTree segTree(nodes);
        ll Q;
        cin >> Q;
        while (Q--)
        {
            ll type, u, val;
            cin >> type;
            if (type == 1)
            {
                cin >> u >> val;
                segTree.update(in[u], out[u], val);
            }
            else
            {
                cin >> u;
                if (isSumOf2Primes(segTree.query(in[u], out[u])))
                    cout << "YES\n";
                else
                    cout << "NO\n";
            }
        }
    }
    return 0;
}
