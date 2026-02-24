#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct WaveletTree
{
#define L (node->l)
#define R (node->r)
#define mid ((node->lo + node->hi) >> 1)
private:
    struct Node
    {
        ll lo, hi;
        Node *l, *r;
        vector<int> b;
        // Constructors
        Node() {}
        Node(ll low, ll high, Node *leftNode, Node *rightNode)
        {
            lo = low;
            hi = high;
            l = leftNode;
            r = rightNode;
        }
    };
    Node *root;

    Node *build(ll *from, ll *to, ll x, ll y)
    {
        Node *node = new Node(x, y, nullptr, nullptr);
        if (from >= to || x == y)
            return node;

        ll m = ((x + y) >> 1);
        auto f = [m](ll c)
        { return c <= m; };

        node->b.push_back(0);
        for (auto it = from; it != to; ++it)
            node->b.push_back(node->b.back() + f(*it));

        auto pivot = stable_partition(from, to, f);

        // Building left node
        L = build(from, pivot, x, m);

        // Building right node
        R = build(pivot, to, m + 1, y);

        return node;
    }
    int countK(Node *node, int leftQuery, int rightQuery, ll k)
    {
        // Out of range
        if (leftQuery > rightQuery || node == nullptr || k < node->lo || k > node->hi)
            return 0;

        // The whole range is the answer
        if (node->lo == node->hi)
            return rightQuery - leftQuery + 1;

        int inLeft = node->b[rightQuery] - node->b[leftQuery - 1];

        // ONLY a part of this segment belongs to the query
        if (k <= mid)
            return countK(L, node->b[leftQuery - 1] + 1, node->b[rightQuery], k);
        else
            return countK(R, leftQuery - node->b[leftQuery - 1], rightQuery - node->b[rightQuery], k);
    }
    int countLessEq(Node *node, int leftQuery, int rightQuery, ll k)
    {
        // Out of range
        if (leftQuery > rightQuery || node == nullptr || k < node->lo)
            return 0;

        // The whole range is the answer
        if (node->hi <= k)
            return rightQuery - leftQuery + 1;

        int inLeft = node->b[rightQuery] - node->b[leftQuery - 1];

        // ONLY a part of this segment belongs to the query
        if (k <= mid)
            return countLessEq(L, node->b[leftQuery - 1] + 1, node->b[rightQuery], k);
        else
            return inLeft + countLessEq(R, leftQuery - node->b[leftQuery - 1], rightQuery - node->b[rightQuery], k);
    }
    ll kthSmallest(Node *node, int leftQuery, int rightQuery, int k)
    {
        // Out of range
        if (leftQuery > rightQuery || node == nullptr)
            return -1;

        // The whole range is the answer
        if (node->lo == node->hi)
            return node->lo;

        int inLeft = node->b[rightQuery] - node->b[leftQuery - 1];

        // ONLY a part of this segment belongs to the query
        if (inLeft >= k)
            return kthSmallest(L, node->b[leftQuery - 1] + 1, node->b[rightQuery], k);
        else
            return kthSmallest(R, leftQuery - node->b[leftQuery - 1], rightQuery - node->b[rightQuery], k - inLeft);
    }
    void destroy(Node *node)
    {
        if (node == nullptr)
            return;
        destroy(node->l);
        destroy(node->r);
        delete node;
    }

public:
    WaveletTree(const vector<ll> &arr)
    {
        if (arr.empty())
        {
            root = nullptr;
            return;
        }

        int n = arr.size();
        ll minVal = *min_element(arr.begin(), arr.end());
        ll maxVal = *max_element(arr.begin(), arr.end());

        vector<ll> temp = arr;
        // Build the tree spanning from the absolute minimum to the absolute maximum
        root = build(&temp[0], &temp[0] + n, minVal, maxVal);
    }
    ~WaveletTree()
    {
        destroy(root);
    }
    int countK(int left, int right, ll k) { return countK(root, left + 1, right + 1, k); }

    int countLess(int left, int right, ll k) { return countLessEq(root, left + 1, right + 1, k - 1); }

    int countLessEq(int left, int right, ll k) { return countLessEq(root, left + 1, right + 1, k); }

    int countGreater(int left, int right, ll k) { return (right - left + 1) - countLessEq(root, left + 1, right + 1, k); }

    int countGreaterEq(int left, int right, ll k) { return (right - left + 1) - countLessEq(root, left + 1, right + 1, k - 1); }

    ll kthSmallest(int left, int right, int k) { return kthSmallest(root, left + 1, right + 1, k); }

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
    cin >> t;
    while (t--)
    {
        int n, k, l, r;
        cin >> n >> k >> l >> r;
        vector<ll> vc(n), last(n);
        map<ll, int> lastPos;
        for (int i{}; i < n; i++)
        {
            cin >> vc[i];
            if (lastPos.count(vc[i]) == 0)
                last[i] = -1;
            else
                last[i] = lastPos[vc[i]];
            lastPos[vc[i]] = i;
        }
        WaveletTree wt(last);
        ll cnt{};
        for (int R = 0, Lk = 0, Lk_1 = 0; R < n; R++)
        {
            while (wt.countLess(Lk, R, Lk) > k)
                Lk++;
            while (wt.countLess(Lk_1, R, Lk_1) >= k)
                Lk_1++;

            ll lenL = R - r + 1, lenR = R - l + 1;
            ll mn = max<ll>(Lk, lenL), mx = min<ll>(Lk_1 - 1, lenR);
            if (mx >= mn)
                cnt += mx - mn + 1;
        }
        cout << cnt << endl;
    }
    return 0;
}