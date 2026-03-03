#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

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
		if (L > R)
			return 0; // Neutral value
		if (L == 0)
			return query(R);
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
	// cin >> t;
	while (t--)
	{
		int N, Q;
		cin >> N >> Q;
		vector<ll> A(N);
		vector<tuple<int, int, int, int, int, int>> queries; // {R, sign, L, x, y, idx}

		for (int i{}; i < N; i++)
			cin >> A[i];

		for (int i = 0; i < Q; i++)
		{
			int L, R, X, Y;
			cin >> L >> R >> X >> Y;
			queries.emplace_back(R, +1, L, X, Y, i);
			queries.emplace_back(L - 1, -1, L, X, Y, i);
		}

		sort(queries.begin(), queries.end());
		vector<ll> result(Q);
		int curr = 0;
		FenwickTree fenTree(2e5);
		for (auto &[r, sign, l, x, y, idx] : queries)
		{
			while (curr < N && curr < r)
			{
				fenTree.update(A[curr], 1);
				++curr;
			}
			ll count = fenTree.rangeQuery(x, y);
			result[idx] += sign * count;
		}

		for (const ll &res : result)
			cout << res << endl;
	}
	return 0;
}