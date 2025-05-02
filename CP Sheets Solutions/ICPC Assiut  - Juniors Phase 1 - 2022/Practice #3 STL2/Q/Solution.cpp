#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

class CompressedRange
{
private:
	vector<ll> init;
	void compress(vector<ll> &vec)
	{
		sort(vec.begin(), vec.end());
		vec.erase(unique(vec.begin(), vec.end()), vec.end());
	}

public:
	CompressedRange(vector<ll> &vec)
	{
		init = vec;
		compress(init);
	}
	int index(ll val)
	{
		return lower_bound(init.begin(), init.end(), val) - init.begin();
	}
	ll initVal(int idx)
	{
		return init[idx];
	}
};

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
	int t = 1;
	ll N, Q;
	// cin >> t;
	while (t--)
	{
		cin >> N >> Q;
		vector<tuple<ll, ll, ll>> vc(N);
		vector<tuple<ll, ll, ll, ll>> queries(Q);
		vector<ll> temp;
		for (auto &[i, j, val] : vc)
		{
			cin >> i >> j >> val;
			temp.push_back(i);
			temp.push_back(j);
		}
		for (auto &[x1, y1, x2, y2] : queries)
		{
			cin >> x1 >> y1 >> x2 >> y2;
			temp.push_back(x1);
			temp.push_back(y1);
			temp.push_back(x2);
			temp.push_back(y2);
		}

		CompressedRange comp(temp);
		for (auto &[i, j, val] : vc)
			i = comp.index(i) + 1, j = comp.index(j) + 1;
		for (auto &[x1, y1, x2, y2] : queries)
		{
			x1 = comp.index(x1) + 1;
			y1 = comp.index(y1) + 1;
			x2 = comp.index(x2) + 1;
			y2 = comp.index(y2) + 1;
		}

		vector<vector<int>> grid(4002, vector<int>(4002, 0));
		vector<vector<ll>> PrefixSum2D(4002, vector<ll>(4002, 0));

		for (auto &[i, j, val] : vc)
			grid[i][j] = val;
		// Simultaneously accumulating rows and columns
		for (size_t i{1}; i < 4000 + 2; i++)
		{
			for (size_t j{1}; j < 4000 + 2; j++)
				PrefixSum2D[i][j] = PrefixSum2D[i - 1][j] + PrefixSum2D[i][j - 1] - PrefixSum2D[i - 1][j - 1] + grid[i][j];
		}

		for (const auto &[x1, y1, x2, y2] : queries)
			cout << PrefixSum2D[x2][y2] - PrefixSum2D[x2][y1 - 1] - PrefixSum2D[x1 - 1][y2] + PrefixSum2D[x1 - 1][y1 - 1] << endl;
	}
	return 0;
}