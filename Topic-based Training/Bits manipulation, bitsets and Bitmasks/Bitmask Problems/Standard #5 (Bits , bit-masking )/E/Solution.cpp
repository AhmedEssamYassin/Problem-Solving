#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif

	short int N;
	cin >> N;
	int *arr = new int[N];
	for (int i{}; i < N; i++)
		cin >> arr[i];

	// Given an array of size N, Count all subsets that have EVEN numbers greater than odd numbers
	int cntSubsets{};
	for (int i = 0; i < (1 << N); i++)
	{
		// Loop through all elements of the input array
		int cntEven{}, cntOdd{};
		for (int j = 0; j < N; j++)
		{
			// Check if the jth bit is set in the current subset
			if (i & (1 << j))
			{
				// If the jth bit is set, add the jth element to the subset
				if (arr[j] & 1) // ODD
					cntOdd++;
				else
					cntEven++;
			}
		}
		if (cntEven > cntOdd)
			cntSubsets++;
	}
	cout << cntSubsets;
	delete[] arr;
	return 0;
}