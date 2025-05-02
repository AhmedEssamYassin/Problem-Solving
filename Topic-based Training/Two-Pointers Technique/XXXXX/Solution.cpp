#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
	int t = 1;
	int N, X, K;
	cin >> t;
	while (t--)
	{
		cin >> N >> K;
		vector<int> rem(N);
		for (int i{}; i < N; i++)
		{
			cin >> X;
			rem[i] = X % K;
		}
		int sum = accumulate(rem.begin(), rem.end(), 0);
		int init = sum;
		int cntFront{}, cntEnd{}, L{}, R{N - 1};
		while (sum % K == 0 && L < N)
			sum -= rem[L++], cntFront++;

		sum = init;
		while (sum % K == 0 && R >= 0)
			sum -= rem[R--], cntEnd++;
		int ans = N - min(cntFront, cntEnd);
		cout << (ans > 0 ? ans : -1) << endl;
	}
	return 0;
}