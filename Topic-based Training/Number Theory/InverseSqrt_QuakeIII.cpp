#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

double inverseSqrt(double x)
{
	// absolute error < 1e-16
	double xHalf = 0.5 * x;
	long long i = *(long long *)&x;		 // store double bits in 64-bit integer
	i = 0x5fe6ec85e7de30daLL - (i >> 1); // magic constant for double precision
	x = *(double *)&i;					 // convert bits back to double

	// 4 rounds of Newton's method
	x = x * (1.5 - xHalf * x * x);
	x = x * (1.5 - xHalf * x * x);
	x = x * (1.5 - xHalf * x * x);
	x = x * (1.5 - xHalf * x * x);
	return x;
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
	ll N;
	// cin >> t;
	while (t--)
	{
		for (int i{1}; i < 1e7; i++)
			assert(fabsl(1.0 / sqrtl(i) - inverseSqrt(i)) < 1e-16);
	}
	return 0;
}
