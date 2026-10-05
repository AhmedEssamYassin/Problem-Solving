#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

// Kadane: max sum non-empty subarray. Returns {sum, L, R}, 0-indexed inclusive: a[L..R]. O(n)
tuple<ll, int, int> maxSubarraySum(const vector<ll> &a)
{
	ll mx = LLONG_MIN, cur = 0;
	int L = 0, R = 0, start = 0;
	for (int i = 0; i < (int)a.size(); i++)
	{
		if (cur <= 0)
			cur = 0, start = i;
		cur += a[i];
		if (cur > mx)
			mx = cur, L = start, R = i;
	}
	return {mx, L, R};
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif

	int t = 1, N;
	cin >> t;
	while (t--)
	{
		cin >> N;
		vector<ll> vc(N);
		for (int i{}; i < N; i++)
			cin >> vc[i];

		tuple<int, int, int> ans = maxSubarraySum(vc);

		cout << get<0>(ans) << " " << get<1>(ans) << " " << get<2>(ans) << endl;
	}
	return 0;
}