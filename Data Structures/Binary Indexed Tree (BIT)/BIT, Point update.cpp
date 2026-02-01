#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

// INVERSION COUNT IN AN ARRAY

/*
Fenwick Tree: Binary Indexed Tree (BIT)
0-based version
Works for invertible operations (such as Summation (+) and XOR (^))
*/
class FenwickTree
{
private:
    int N;
    vector<ll> BIT;

public:
    FenwickTree(int sz)
    {
        N = sz;
        BIT.assign(N + 1, 0);
    }
    void update(int pos, const int &delta)
    {
        for (int i = pos; i < N; i |= (i + 1))
            BIT[i] += delta;
    }

    ll query(int pos)
    {
        ll sum = 0;
        for (int i = pos; i >= 0; i = (i & (i + 1)) - 1)
            sum += BIT[i];

        return sum;
    }

    ll rangeQuery(int L, int R)
    {
        if (L > R || L <= 0)
            return 0; // Neutral value
        return (query(R) - query(L - 1));
    }
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
        int N;
        cin >> N;
        vector<ll> arr(N + 1);
        FenwickTree fenTree(N);
        // Inputting and initializing
        for (int i = 1; i <= N; i++)
            cin >> arr[i];

        int res = 0;
        ll ans = 0;
        for (int i = 1; i <= N; i++)
        {
            ans += i - 1 - fenTree.query(arr[i] - 1);
            fenTree.update(arr[i], 1);
        }
        cout << ans << endl;
    }
    return 0;
}