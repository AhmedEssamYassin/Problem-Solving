#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

void print(int N)
{
	if (N == 0)
		return;
	cout << N;
	if (N > 1)
		cout << " ";
	print(N - 1);
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
	int N;
	// cin >> t;
	while (t--)
	{
		cin >> N; // Number of lines
		print(N);
	}
	return 0;
}