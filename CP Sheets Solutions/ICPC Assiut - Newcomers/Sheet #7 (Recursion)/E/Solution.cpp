#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

void printDigits(long long N)
{
	if (N < 2)
	{
		cout << N;
		return;
	}
	printDigits(N / 2);
	cout << N % 2;
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
	cin >> t;
	while (t--)
	{
		cin >> N; // Number of lines
		printDigits(N);
		cout << endl;
	}
	return 0;
}