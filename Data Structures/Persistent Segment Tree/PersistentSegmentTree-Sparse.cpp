#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct PersistentSegmentTree
{
#define mid ((left + right) >> 1)
private:
    struct Node
    {
        ll value;
        int L;
        int R;
        Node() { value = 0, L = 0, R = 0; }
        Node(const ll &x)
        {
            value = x;
            L = 0;
            R = 0;
        }
        void merge(const Node &leftNode, const Node &rightNode)
        {
            value = (leftNode.value + rightNode.value);
        }
    };

    vector<Node> tree;
    ll minValue, maxValue;
    vector<int> versions;

    int build(const vector<ll> &arr, ll left, ll right)
    {
        int newNode = tree.size();
        tree.push_back(Node());

        if (left == right)
        {
            tree[newNode] = arr[left];
            return newNode;
        }

        tree[newNode].L = build(arr, left, mid);
        tree[newNode].R = build(arr, mid + 1, right);
        tree[newNode].merge(tree[tree[newNode].L], tree[tree[newNode].R]);
        return newNode;
    }

    int insert(int node, ll left, ll right, ll idx, ll val)
    {
        int newNode = tree.size();
        tree.push_back(node ? tree[node] : Node());

        if (left == right)
        {
            tree[newNode].value += val;
            return newNode;
        }

        if (idx <= mid)
            tree[newNode].L = insert(tree[newNode].L, left, mid, idx, val);
        else
            tree[newNode].R = insert(tree[newNode].R, mid + 1, right, idx, val);

        Node leftChild = tree[newNode].L ? tree[tree[newNode].L] : Node();
        Node rightChild = tree[newNode].R ? tree[tree[newNode].R] : Node();
        tree[newNode].merge(leftChild, rightChild);

        return newNode;
    }

    Node query(int node, ll left, ll right, ll leftQuery, ll rightQuery)
    {
        if (!node || leftQuery > right || rightQuery < left)
            return Node();
        if (leftQuery <= left && right <= rightQuery)
            return tree[node];

        Node leftSegment = query(tree[node].L, left, mid, leftQuery, rightQuery);
        Node rightSegment = query(tree[node].R, mid + 1, right, leftQuery, rightQuery);
        Node res;
        res.merge(leftSegment, rightSegment);
        return res;
    }

    ll getKth(int nodeL, int nodeR, ll left, ll right, int k)
    {
        if (left == right)
            return left;

        int countLeft = tree[tree[nodeR].L].value - tree[tree[nodeL].L].value;

        if (countLeft >= k)
            return getKth(tree[nodeL].L, tree[nodeR].L, left, mid, k);
        return getKth(tree[nodeL].R, tree[nodeR].R, mid + 1, right, k - countLeft);
    }

public:
    PersistentSegmentTree(ll minValue, ll maxValue, int maxInsertions) : minValue(minValue), maxValue(maxValue)
    {
        ll maxRange = maxValue - minValue + 1;
        int capacity = maxInsertions * (__lg(max(1LL, maxRange)) + 4) + 2;
        tree.reserve(capacity);
        tree.push_back(Node());
        int root = tree.size();
        tree.push_back(Node());
        versions.push_back(root);
    }

    // Constructs tree from base array. Returns new version index.
    // Call ONCE initially if tree isn't sparse.
    int build(const vector<ll> &arr)
    {
        tree.reserve(tree.capacity() + 4 * arr.size());
        int root = build(arr, minValue, maxValue);
        versions.push_back(root);
        return versions.size() - 1;
    }

    // Inserts `value` at `idx` based on a previous version. Returns new version index.
    // Call to create a new historical state
    int insert(int version, ll i, ll value)
    {
        assert(version >= 0 && version < versions.size());
        int newRoot = insert(versions[version], minValue, maxValue, i, value);
        versions.push_back(newRoot);
        return versions.size() - 1;
    }

    // Updates a version IN-PLACE. Does NOT create a new version.
    // Call when modifying a state without needing to keep the old one.
    void update(int version, ll i, ll val)
    {
        assert(version >= 1 && version < versions.size());
        // Overwrite the root of the existing version instead of appending a new one
        versions[version] = insert(versions[version], minValue, maxValue, i, val);
    }

    // Queries sum/result in [leftQuery, rightQuery] for a specific version.
    // Call to answer standard range queries on historical data.
    ll query(int version, ll leftQuery, ll rightQuery)
    {
        assert(version >= 0 && version < versions.size());
        return query(versions[version], minValue, maxValue, leftQuery, rightQuery).value;
    }

    // Finds the K-th smallest element in [versionL, versionR]
    // Call for that type of problems where tree stores value frequencies.
    ll getKth(int versionL, int versionR, int k)
    {
        assert(versionL > 0 && versionL < versions.size());
        assert(versionR >= 0 && versionR < versions.size());
        return getKth(versions[versionL - 1], versions[versionR], minValue, maxValue, k);
    }

    // Duplicates a specific version exactly. Returns new version index.
    // Call when a time-step occurs but no actual data changes.
    int copyAndAppend(int k)
    {
        assert(k >= 0 && k < versions.size());
        versions.push_back(versions[k]);
        return versions.size() - 1;
    }

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
        int N, Q;
        cin >> N >> Q;
        vector<ll> vc(N);
        for (int i{}; i < N; i++)
            cin >> vc[i];
        PersistentSegmentTree pst(-1e9, 1e9, N);
        for (int i{}; i < N; i++)
            pst.insert(i, vc[i], 1);

        while (Q--)
        {
            int L, R, k;
            cin >> L >> R >> k;
            cout << pst.getKth(L, R, k) << endl;
        }
    }
    return 0;
}