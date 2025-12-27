#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

vector<int> mu;
void preComputeMobius(ll N)
{
	mu.resize(N + 1);
	mu[1] = -1;
	for (int i = 1; i <= N; i++)
	{
		if (mu[i])
		{
			mu[i] = -mu[i];
			for (int j = 2 * i; j <= N; j += i)
				mu[j] += mu[i];
		}
	}
}

static int autoCall = (preComputeMobius(2e6), 0);

ll cntCoprimePairs(vector<int> &arr)
{
	int maxVal = *max_element(arr.begin(), arr.end());
	vector<int> freq(maxVal + 1, 0), d(maxVal + 1, 0);

	for (int x : arr)
		freq[x]++;

	for (int k = 1; k <= maxVal; k++)
		for (int j = k; j <= maxVal; j += k)
			d[k] += freq[j];

	ll cnt = 0;
	for (int k = 1; k <= maxVal; k++)
	{
		if (mu[k] == 0 || d[k] < 2)
			continue;
		ll pairs = 1LL * d[k] * (d[k] - 1) / 2;
		cnt += 1LL * mu[k] * pairs;
	}

	return cnt;
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
	// cin >> t;
	while (t--)
	{
		int n;
		cin >> n;
		vector<int> vc(n);
		for (int i{}; i < n; i++)
			cin >> vc[i];
		cout << cntCoprimePairs(vc);
	}
	return 0;
}