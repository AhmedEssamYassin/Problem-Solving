#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

bool isOk(const vector<ll> &vc, int d, long double mid, int &segL, int &segR)
{
	int n = (int)vc.size();
	vector<long double> pref(n + 1, 0);
	for (int i = 1; i <= n; i++)
		pref[i] = pref[i - 1] + (vc[i - 1] - mid);

	long double minPref = 0;
	int minIdx = 0;
	for (int r = d; r <= n; r++)
	{
		if (pref[r - d] < minPref)
		{
			minPref = pref[r - d];
			minIdx = r - d;
		}
		if (pref[r] - minPref >= 0)
		{
			segL = minIdx + 1;
			segR = r;
			return true;
		}
	}
	return false;
}

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
		int n, d;
		cin >> n >> d;
		vector<ll> vc(n);
		for (int i{}; i < n; i++)
			cin >> vc[i];

		long double L = 0, R = 100;
		int bestL = 1, bestR = d, curL = 1, curR = d;
		for (int i{}; i < 80; i++)
		{
			long double mid = (L + R) / 2;
			if (isOk(vc, d, mid, curL, curR))
			{
				L = mid;
				bestL = curL;
				bestR = curR;
			}
			else
				R = mid;
		}
		cout << bestL << " " << bestR;
	}
	return 0;
}
