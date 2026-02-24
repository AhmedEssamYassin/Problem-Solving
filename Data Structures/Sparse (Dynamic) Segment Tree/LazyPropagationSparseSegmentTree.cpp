#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct LazyDynamicSegmentTree
{
#define mid ((left + right) >> 1)
private:
    struct Node
    {
        ll value;
        Node *L, *R; // Pointers to left child and right child

        Node() // Constructor
        {
            value = 0;
            L = nullptr;
            R = nullptr;
        }
        Node(const ll &val)
        {
            value = val;
            L = nullptr;
            R = nullptr;
        }
        void createChildren(const ll &val = 0) // Construct Childs
        {
            if (L == nullptr)
                L = new Node(val);
            if (R == nullptr)
                R = new Node(val);
        }
        ~Node() // Destructor. Notice the "~" character before the struct name.
        {
            delete L;
            delete R;
            L = nullptr;
            R = nullptr;
        }
    };
    ll minValue, maxValue;
    Node *segRoot;
    Node *lazyRoot;
    void merge(Node *&segNode)
    {
        segNode->value = (segNode->L->value + segNode->R->value);
    }

    void push(ll left, ll right, Node *segNode, Node *lazyNode)
    {
        // Propagate the value
        if (segNode == nullptr || lazyNode == nullptr || lazyNode->value == -1)
            return;
        // (a + b + c + d + e) --> (x + x + x + x + x) --> (right - left + 1) * x
        segNode->value = (right - left + 1) * lazyNode->value;
        // If the node is not a leaf
        if (left != right)
        {
            lazyNode->createChildren();
            // Update the lazy values for the left child
            lazyNode->L->value = lazyNode->value;

            // Update the lazy values for the right child
            lazyNode->R->value = lazyNode->value;
        }
        // Reset the lazy value
        lazyNode->value = -1;
    }
    void update(ll left, ll right, Node *&segNode, Node *&lazyNode, ll leftQuery, ll rightQuery, const ll &val)
    {
        push(left, right, segNode, lazyNode);
        // If the range is invalid, return
        if (left > rightQuery || right < leftQuery)
            return;
        // If the range matches the segment
        if (left >= leftQuery && right <= rightQuery)
        {
            // Update the lazy value
            lazyNode->value = val;
            // Apply the update immediately
            push(left, right, segNode, lazyNode);
            return;
        }
        segNode->createChildren(0);
        lazyNode->createChildren(-1);
        // Recursively update the left child
        update(left, mid, segNode->L, lazyNode->L, leftQuery, rightQuery, val);
        // Recursively update the right child
        update(mid + 1, right, segNode->R, lazyNode->R, leftQuery, rightQuery, val);
        // Merge the children values
        merge(segNode);
    }
    ll query(ll left, ll right, Node *segNode, Node *lazyNode, ll leftQuery, ll rightQuery)
    {
        // If the range is invalid, return a value that does NOT to affect other queries
        if (left > rightQuery || right < leftQuery)
            return 0;

        // Apply the pending updates if any
        push(left, right, segNode, lazyNode);
        segNode->createChildren();
        lazyNode->createChildren(-1);
        // If the range matches the segment
        if (leftQuery <= left && right <= rightQuery)
            return segNode->value;
        ll leftSegment = query(left, mid, segNode->L, lazyNode->L, leftQuery, rightQuery);
        ll rightSegment = query(mid + 1, right, segNode->R, lazyNode->R, leftQuery, rightQuery);
        return leftSegment + rightSegment;
    }

public:
    LazyDynamicSegmentTree(ll minValue = 0, ll maxValue = 1e9) : minValue(minValue), maxValue(maxValue)
    {
        segRoot = new Node();
        lazyRoot = new Node(-1);
    }
    void update(ll left, ll right, const ll &val)
    {
        update(minValue, maxValue, segRoot, lazyRoot, left, right, val);
    }
    ll query(ll left, ll right)
    {
        ll ans = query(minValue, maxValue, segRoot, lazyRoot, left, right);
        return ans;
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
    ll N, M;
    // cin >> t;
    while (t--)
    {
        cin >> N >> M;
        LazyDynamicSegmentTree segTree(0, 1e5);
        while (M--)
        {
            ll query, L, R, i, v;
            cin >> query >> L >> R;
            if (query == 1)
            {
                cin >> v;
                segTree.update(L, R - 1, v);
            }
            else
                cout << segTree.query(L, R - 1) << endl;
        }
    }
    return 0;
}