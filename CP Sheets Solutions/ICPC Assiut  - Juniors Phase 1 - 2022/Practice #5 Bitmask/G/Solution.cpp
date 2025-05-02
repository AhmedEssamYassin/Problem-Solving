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
	int t = 1, N;
	// cin >> t;
	while (t--)
	{
		/*
		At every B[i][j] = (A[i] & A[j]),
		that means, for every set bit in B[i][j], it MUST be also set in both A[i] and A[j]
		We need to construct ALL A[i]'s, so for every B[i][j] = (A[i] & A[j]) and (i != j), we will set ALL Kth set bits in A[i]
		We can easily do this for each A[i] using (A[i] |= B[i][j])
		*/
		int Bij, Ai;
		cin >> N;
		for (int i{}; i < N; i++)
		{
			Ai = 0; // To construct Ai
			for (int j{}; j < N; j++)
			{
				cin >> Bij;
				if (i != j)
					Ai |= Bij;
			}
			cout << Ai << " ";
		}
	}
	return 0;
}