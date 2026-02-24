#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct DynamicSegmentTree
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

        ~Node() // Destructor. Notice the "~" character before the struct name.
        {
            delete L;
            delete R;
            L = nullptr;
            R = nullptr;
        }
    };
    ll minValue, maxValue;
    Node *root;
    void merge(Node *&segNode)
    {
        segNode->value = (segNode->L ? segNode->L->value : 0) + (segNode->R ? segNode->R->value : 0);
    }
    void update(ll left, ll right, Node *&curr, ll idx, const ll &newValue)
    {
        // idx is not in range [left, right]
        if (left > idx || right < idx)
            return;
        if (curr == nullptr)
            curr = new Node();
        // curr is the Node that manage only ith element
        if (left == right)
        {
            curr->value += newValue;
            return;
        }

        update(left, mid, curr->L, idx, newValue);
        update(mid + 1, right, curr->R, idx, newValue);
        merge(curr);
    }
    ll query(ll left, ll right, Node *curr, ll leftQuery, ll rightQuery)
    {
        // [left, right] doesn't intersect with [leftQuery, rightQuery]
        if (curr == nullptr || left > rightQuery || right < leftQuery)
            return 0;

        // [left, curR] is inside [leftQuery, rightQuery]
        if (leftQuery <= left && right <= rightQuery)
            return curr->value;

        ll leftSegment = query(left, mid, curr->L, leftQuery, rightQuery);
        ll rightSegment = query(mid + 1, right, curr->R, leftQuery, rightQuery);
        return (leftSegment + rightSegment);
    }

public:
    DynamicSegmentTree(ll minValue = 0, ll maxValue = 1e9) : minValue(minValue), maxValue(maxValue) { root = new Node(); }

    void update(ll idx, const ll &val)
    {
        update(minValue, maxValue, root, idx, val);
    }
    ll query(ll left, ll right)
    {
        ll ans = query(minValue, maxValue, root, left, right);
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
    ll N;
    // cin >> t;
    while (t--)
    {
        cin >> N;
        map<ll, DynamicSegmentTree> mp;

        ll type, time, value;
        for (int i{}; i < N; i++)
        {
            cin >> type >> time >> value;
            if (type == 1) // Insert
                mp[value].update(time, 1);
            else if (type == 2) // Delete
                mp[value].update(time, -1);
            else // Count
                cout << mp[value].query(1, time) << endl;
        }
    }
    return 0;
}