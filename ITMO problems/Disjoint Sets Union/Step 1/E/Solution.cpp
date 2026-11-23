#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct DSU
{
	vector<int> parent, size; // Representative
	vector<vector<int>> members;
	int N;
	// The leader should be the parent of itself
	DSU(int _N)
	{
		N = _N;
		parent.resize(N + 1);
		size.resize(N + 1, 1); // Each component is of size 1 initially
		members.resize(N + 1);
		iota(parent.begin(), parent.end(), 0);
		for (int i = 1; i <= N; i++)
			members[i].push_back(i);
	}
	int Find(int node)
	{
		if (parent[node] == node)
			return node;

		return parent[node] = Find(parent[node]); // Path compression
	}
	void Union(int u, int v, int curSec, vector<int> &fallTime)
	{
		int rep1 = Find(u);
		int rep2 = Find(v);
		if (rep1 == rep2)
			return;
		int repOne = Find(1);
		if (curSec != -1)
		{
			if (rep1 == repOne)
			{
				for (int x : members[rep2])
					fallTime[x] = curSec;
			}
			else if (rep2 == repOne)
			{
				for (int x : members[rep1])
					fallTime[x] = curSec;
			}
		}
		// Small-to-large technique
		if (size[rep1] > size[rep2])
			swap(rep1, rep2); // representative of smaller set comes first
		parent[rep1] = rep2;
		size[rep2] += size[rep1];
		for (int x : members[rep1])
			members[rep2].push_back(x);
		members[rep1].clear();
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
		int n, m;
		cin >> n >> m;
		vector<array<int, 2>> hands(n + 1);
		vector<array<bool, 2>> released(n + 1, {false, false});
		for (int i = 1; i <= n; i++)
			cin >> hands[i][0] >> hands[i][1];

		vector<pair<int, int>> ops(m);
		for (int i{}; i < m; i++)
		{
			cin >> ops[i].first >> ops[i].second;
			released[ops[i].first][ops[i].second - 1] = true;
		}

		DSU disjointUnion(n);
		vector<int> fallTime(n + 1, -1);
		for (int i = 1; i <= n; i++)
		{
			for (int h = 0; h < 2; h++)
			{
				if (!released[i][h] && hands[i][h] != -1)
					disjointUnion.Union(i, hands[i][h], -1, fallTime);
			}
		}
		for (int sec = m - 1; sec >= 0; sec--)
		{
			auto [p, h] = ops[sec];
			int other = hands[p][h - 1];
			if (other != -1)
				disjointUnion.Union(p, other, sec, fallTime);
		}
		for (int i = 1; i <= n; i++)
			cout << fallTime[i] << endl;
	}
	return 0;
}
