#include <bits/stdc++.h>
#include <span>
using namespace std;
#define ll long long int
#define endl "\n"

vector<int> divFlat, divStart; // divisors of j: divFlat[divStart[j] .. divStart[j + 1])
void precomputeDivisors(int N)
{
	divStart.assign(N + 2, 0);
	for (int i = 1; i <= N; i++)
		for (int j = i; j <= N; j += i)
			divStart[j]++;
	partial_sum(divStart.begin(), divStart.end(), divStart.begin()); // divStart[j] = end of j's range
	divFlat.resize(divStart[N + 1]);
	for (int i = N; i >= 1; i--)
		for (int j = i; j <= N; j += i)
			divFlat[--divStart[j]] = i;
}
static int run_ = (precomputeDivisors(1.5e6), 0);
inline span<int> divsOf(int v) { return {divFlat.data() + divStart[v], divFlat.data() + divStart[v + 1]}; }

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
	}
	return 0;
}
