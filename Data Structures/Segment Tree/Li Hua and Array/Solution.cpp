#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

vector<int> primes;
vector<int> phi, depth;
void precomputePhi(int N)
{
    depth.assign(N + 1, 0);
    phi.assign(N + 1, 0);
    phi[1] = 1;
    for (long long i = 2; i <= N; i++)
    {
        if (phi[i] == 0)
        {
            phi[i] = i - 1;
            primes.push_back(i);
        }
        for (const int &p : primes)
        {
            if (i * p > N)
                break;
            if (i % p == 0)
            {
                phi[i * p] = phi[i] * p;
                break;
            }
            phi[i * p] = phi[i] * (p - 1);
        }
    }
    depth[1] = 0;
    for (int i = 2; i <= N; i++)
        depth[i] = depth[phi[i]] + 1;
}

static int autoCall = (precomputePhi(5e6), 0);

int getLCA(int u, int v)
{
    if (!u || !v)
        return u ? u : v;
    while (u ^ v)
    {
        if (depth[u] < depth[v])
            swap(u, v);
        u = phi[u];
    }
    return u;
}

struct SegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
    struct Node
    {
        int mx, sumDepths, LCA;
        // Constructors
        Node() { mx = INT_MIN, sumDepths = 0, LCA = 0; }
        Node(int x) { mx = x, sumDepths = depth[x], LCA = x; }
    };
    int size;
    vector<Node> seg;
    Node merge(const Node &leftNode, const Node &rightNode)
    {
        Node res;
        res.mx = max(leftNode.mx, rightNode.mx);
        res.sumDepths = (leftNode.sumDepths + rightNode.sumDepths);
        res.LCA = getLCA(leftNode.LCA, rightNode.LCA);
        return res;
    }
    void build(int left, int right, int node, const vector<ll> &arr)
    {
        if (left == right)
        {
            if (left < arr.size())
                seg[node] = arr[left];
            return;
        }
        // Building left node
        build(left, mid, L, arr);

        // Building right node
        build(mid + 1, right, R, arr);

        // Returning to parent nodes
        seg[node] = merge(seg[L], seg[R]);
    }
    void update(int left, int right, int node, int leftQuery, int rightQuery)
    {
        if (left > rightQuery || right < leftQuery || seg[node].mx == 1)
            return;
        if (left == right)
        {
            seg[node] = phi[seg[node].mx];
            return;
        }
        update(left, mid, L, leftQuery, rightQuery);
        update(mid + 1, right, R, leftQuery, rightQuery);
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
    SegmentTree(const vector<ll> &arr)
    {
        size = 1;
        int n = arr.size();
        while (size < n)
            size <<= 1;
        seg = vector<Node>(2 * size);
        build(0, size - 1, 0, arr);
    }
    void update(int left, int right)
    {
        update(0, size - 1, 0, left, right);
    }
    pair<int, int> query(int left, int right)
    {
        Node ans = query(0, size - 1, 0, left, right);
        return {ans.sumDepths, ans.LCA};
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
        int n, m;
        cin >> n >> m;
        vector<ll> vc(n);
        for (int i{}; i < n; i++)
            cin >> vc[i];
        SegmentTree segTree(vc);
        while (m--)
        {
            int type, l, r;
            cin >> type >> l >> r;
            --l, --r;
            if (type == 1)
                segTree.update(l, r);
            else
            {
                const auto &[sumDepths, LCA] = segTree.query(l, r);
                ll ans = sumDepths - (r - l + 1) * depth[LCA];
                cout << ans << endl;
            }
        }
    }
    return 0;
}