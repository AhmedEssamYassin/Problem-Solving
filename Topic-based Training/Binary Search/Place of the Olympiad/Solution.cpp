#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

bool isOk(ll N, ll M, ll len, ll K)
{
	// We put benches of length `len`
	// The remaining seats can hold students alternately.
	ll cntBenches = M / (len + 1) + (M % (len + 1) == len);
	ll cntSeats = cntBenches * len * N;
	bool include = M % (len + 1) != len;
	return (cntSeats + include * (M % (len + 1) * N) >= K);
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
	int t = 1;
	ll N, M, K;
	cin >> t;
	while (t--)
	{
		cin >> N >> M >> K;
		ll L{1}, R = (1LL << 32), ans = M;
		while (L <= R)
		{
			ll mid = ((L + R) >> 1);
			if (isOk(N, M, mid, K))
				ans = mid, R = mid - 1;
			else
				L = mid + 1;
		}
		cout << ans << endl;
	}
	return 0;
}