#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

long double probability(ll N, long double p)
{
	long double probabilityH{1.0};
	while (N)
	{
		if (N & 1)
			probabilityH = probabilityH * (1 - p) + (1 - probabilityH) * p;
		p = p * (1 - p) + (1 - p) * p;
		N >>= 1;
	}
	return probabilityH;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
	ll N;
	long double p;
	cin >> N >> p;
	cout << fixed << setprecision(14) << probability(N, p);
	return 0;
}