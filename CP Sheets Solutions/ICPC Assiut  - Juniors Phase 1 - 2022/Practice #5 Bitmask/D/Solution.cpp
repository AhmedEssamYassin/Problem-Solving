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

	/*
	A problem set for the contest must consist of AT LEAST two problems.
	You think that the total difficulty of the problems of the contest must be at least L and at most R.
	Also, you think that the difference between difficulties of the easiest and the hardest of the chosen problems must be AT LEAST X.

	Find the number of ways to choose a problem set for the contest.
	Since N is small (N <= 15), we can generate ALL possible subsets (2^15 subsets) using a Bitmask

	Assume S = {1, 2, 2}
	The binary digits from 0 to 7 are

	0 –> 000    –> number formed with no setbits                          –> { }
	1 –> 001    –> number formed with setbit at position 0                –> { 1 }
	2 –> 010    –> number formed with setbit at position 1                –> { 2 }
	3 –> 011    –> number formed with setbit at position 0  and 1         –> { 1 , 2 }
	4 –> 100    –> number formed with setbit at position 2                –>  { 2 }
	5 –> 101    –> number formed with setbit at position 0 and 2      	  –> { 1 , 2}
	6 –> 110    –> number formed with setbit at position 1 and 2          –> { 2 , 2}
	7 –> 111    –> number formed with setbit at position 0 , 1 and 2      –> {1 , 2 , 2}
	After removing duplicates final result will be { }, { 1 }, { 2 }, { 1 , 2 }, { 2 , 2 }, { 1 , 2 , 2}
	*/
	int N, L, R, X;
	int arr[16];
	cin >> N >> L >> R >> X;
	for (int i{}; i < N; i++)
		cin >> arr[i];

	int Ans = 0;
	for (int Bitmask = 0; Bitmask < (1 << N); Bitmask++)
	{
		int difficulty = 0, hardest = 0, easiest = 10000000, cntProblems = 0;
		for (int ith_bit = 0; ith_bit < N; ith_bit++)
		{
			if (Bitmask & (1 << ith_bit)) // If ith_bit is set
			{
				difficulty += arr[ith_bit];
				hardest = max(hardest, arr[ith_bit]);
				easiest = min(easiest, arr[ith_bit]);
				cntProblems++;
			}
		}
		if (difficulty >= L && difficulty <= R && hardest - easiest >= X && cntProblems >= 2)
			Ans++;
	}
	cout << Ans;
	return 0;
}