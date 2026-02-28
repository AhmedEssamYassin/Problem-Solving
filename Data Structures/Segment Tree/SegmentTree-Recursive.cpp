#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

/*
Parent Node : A node with NO parent
Leaf Node: A node with NO children
Siblings: Nodes that are of the same level in the heirarchy

At level of Leafs, each node is responsible for one node (single element) (0 trailing zeros)
in its higher level, each node is responsible for 2 nodes (1 trailing zero)
in its higher level, each node is responsible for 4 nodes (2 trailing zeros)
in its higher level, each node is responsible for 8 nodes (3 trailing zeros)
and so on

So we can alternate between levels based on the parity of number of trailing zeros.
*/

struct SegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
    struct Node
    {
        ll sum;
        // Constructors
        Node() { sum = 0; }
        Node(ll val) { sum = val; }
    };
    int size;
    vector<Node> seg;
    Node merge(const Node &leftNode, const Node &rightNode)
    {
        Node res;
        res.sum = (leftNode.sum + rightNode.sum);
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
    void update(int left, int right, int node, int idx, const ll &val)
    {
        if (left == right)
        {
            seg[node] = val;
            return;
        }
        if (idx <= mid)
            update(left, mid, L, idx, val);
        else
            update(mid + 1, right, R, idx, val);
        // Updating while returning to parent nodes
        seg[node] = merge(seg[L], seg[R]);
    }
    Node query(int left, int right, int node, int leftQuery, int rightQuery)
    {
        // Out of range
        if (right < leftQuery || left > rightQuery)
            return Node(); // A neutral value that doesn't affect other queries

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
    void update(int idx, const ll &val)
    {
        update(0, size - 1, 0, idx, val);
    }
    ll query(int left, int right)
    {
        Node ans = query(0, size - 1, 0, left, right);
        return ans.sum;
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
    ll N, M, q, i, v, L, R;
    cin >> N >> M;
    vector<ll> vc(N);
    for (int i{}; i < N; i++)
        cin >> vc[i];
    SegmentTree segTree(vc);
    while (M--)
    {
        cin >> q;
        if (q == 1)
        {
            cin >> i >> v;
            segTree.update(i, v);
        }
        else
        {
            cin >> L >> R;
            cout << segTree.query(L, R - 1) << endl;
        }
    }

    return 0;
}