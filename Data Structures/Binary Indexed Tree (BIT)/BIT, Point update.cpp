#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

// INVERSION COUNT IN AN ARRAY

/*
Fenwick Tree: Binary Indexed Tree (BIT)
1-based (because (0 & -0) would make an infinite loop)
Works for invertible operations (such as Summation (+) and XOR (^))
*/
int N;
int BIT[200010];

void Update(int pos, const int &delta)
{
    for (int i = pos; i <= N; i += (i & -i))
        BIT[i] += delta;
}

ll query(int pos)
{
    ll sum = 0;
    for (int i = pos; i; i -= (i & -i))
        sum += BIT[i];

    return sum;
}

ll rangeQuery(int L, int R)
{
    return (query(R) - query(L));
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
    int t = 1;
    cin >> t;
    while (t--)
    {
        cin >> N;
        int *arr = new int[N + 1];
        // Inputting and initializing
        for (int i = 1; i <= N; i++)
            cin >> arr[i], BIT[i] = 0;

        int res = 0;
        ll ans = 0;
        for (int i = 1; i <= N; i++)
        {
            ans += i - 1 - query(arr[i] - 1);
            Update(arr[i], 1);
        }
        cout << ans << endl;
    }
    return 0;
}