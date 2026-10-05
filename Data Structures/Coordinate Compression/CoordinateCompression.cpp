#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

class CoordinateCompression
{
	vector<ll> vals; // sorted, unique
public:
	CoordinateCompression() {}
	explicit CoordinateCompression(const vector<ll> &v) : vals(v)
	{
		sort(vals.begin(), vals.end());
		vals.erase(unique(vals.begin(), vals.end()), vals.end());
	}
	int size() const { return vals.size(); }
	int index(ll v) const { return lower_bound(vals.begin(), vals.end(), v) - vals.begin(); }
	ll value(int i) const { return vals[i]; }
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
	int N;
	// cin >> t;
	while (t--)
	{
		cin >> N;
		vector<ll> L(N), R(N), idx;
		for (int i{}; i < N; i++)
		{
			cin >> L[i] >> R[i];
			idx.push_back(L[i]);
			idx.push_back(R[i]);
		}

		CoordinateCompression comp(idx);

		for (int i{}; i < N; i++)
		{
			L[i] = comp.index(L[i]);
			R[i] = comp.index(R[i]);
		}

		int *scanLine{new int[2 * N + 1]{0}};
		for (int i{}; i < N; i++)
		{
			scanLine[L[i]]++;
			scanLine[R[i] + 1]--;
		}

		int maxAns{scanLine[0]};
		for (int i{1}; i < 2 * N + 1; i++)
		{
			scanLine[i] += scanLine[i - 1];
			if (scanLine[i] > maxAns)
				maxAns = scanLine[i];
		}

		cout << maxAns;
	}
	return 0;
}