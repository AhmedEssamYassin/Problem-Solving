#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

void print(const ll &N)
{
	for (int i = 1; i < N; i++)
		cout << i << " ";
	cout << N; // To avoid the extra space
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
		print(N);
	}
	return 0;
}