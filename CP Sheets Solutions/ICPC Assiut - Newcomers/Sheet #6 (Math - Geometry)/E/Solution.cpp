#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif
	/*
	This is basically the summation of numbers from 1 to N
	N * (N + 1) / 2 <= X
	N² + N - 2 * X = 0
	*/
	int t = 1;
	ll N, X;
	// cin >> t;
	while (t--)
	{
		cin >> X;
		N = (-1 + sqrtl(1 + 8 * X)) / 2;
		cout << N;
	}
	return 0;
}