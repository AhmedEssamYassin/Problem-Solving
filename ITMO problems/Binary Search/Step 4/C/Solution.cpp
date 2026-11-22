#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

bool isOk(const vector<ll> &a, const vector<ll> &b, int k, long double mid, vector<long double> &val)
{
	int n = (int)a.size();
	for (int i{}; i < n; i++)
		val[i] = a[i] - mid * b[i];
	nth_element(val.begin(), val.begin() + k, val.end(), greater<>());
	long double sum = 0;
	for (int i{}; i < k; i++)
		sum += val[i];
	return sum >= 0;
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
		int n, k;
		cin >> n >> k;
		vector<ll> a(n), b(n);
		for (int i{}; i < n; i++)
			cin >> a[i] >> b[i];

		long double L = 0, R = 1e5;
		vector<long double> val(n);
		for (int i{}; i < 80; i++)
		{
			long double mid = (L + R) / 2;
			if (isOk(a, b, k, mid, val))
				L = mid;
			else
				R = mid;
		}
		cout << fixed << setprecision(10) << L;
	}
	return 0;
}
