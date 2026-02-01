#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

void maxLeft(vector<ll> &arr, int N)
{
	if (N <= 0)
		return;
	maxLeft(arr, N - 1);
	arr[N] = max(arr[N - 1], arr[N]);
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
		cin >> N;
		vector<ll> vc(N);
		for (int i{}; i < N; i++)
			cin >> vc[i];
		maxLeft(vc, N - 1);
		for (int i{}; i < N; i++)
			cout << vc[i] << " ";
	}
	return 0;
}