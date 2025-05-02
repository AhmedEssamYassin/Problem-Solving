#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

int minLength_maxOR(vector<int> &nums, int &max_OR)
{
	int bitmask[30] = {0}; // Enough for N <= 1e9
	int minLen{INT_MAX};
	for (int i = nums.size() - 1; i >= 1; --i) // Traversing numbers in reverse order (From back to front)
	{
		int OR{}; // To be constructed

		const int num = nums[i]; // Current number

		for (int j = 0; j < 30; ++j) // Traversing ALL bits
		{
			if (num & (1 << j)) // If that number can set that bit
				bitmask[j] = i; // store that index
		}

		for (int k = 0; k < 30; ++k) // Traversing ALL bits
		{
			if (bitmask[k])		// If that bit is set
				OR |= (1 << k); // set that bit in OR
		}

		const int maxIdx = *max_element(bitmask, bitmask + 30); // The farthest element who is capable of setting one of the bits
		/*
		We choose the maximum index because we are obliged to include that element in current sub_array!
		There are two main reasons for that:
		1. If we excluded it, That means, there will exist a better answer than ours!
		   because another sub_array would have greater value by including ALL current elements besides that excluded element.

		2. If there existed any nearer element who is capable of setting the same bits instead of that number,
		   it would have been included and overwritten its index because we are traversing in reverse order!
		*/
		if (OR == max_OR)
			minLen = min(minLen, max(maxIdx, i) - i + 1);
		// We choose max(maxIdx, i) because if NO bits are set, that means ALL bits have indices 0 now
		// In that case, it's always optimal to have a sub_array of length 1 which is ONLY one element
	}
	return minLen;
}

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
		cin >> N;

		// Make indices 1-based, because later, indices will have a boolean semantic and the index 0 will corrupt the evaluation.
		vector<int> arr(N + 1);

		// The bitwise OR yields NON-decreasing values, i.e., each value is >= its preceding values
		// So it's easy to see that ORing ALL elements yields the maximum possible OR value
		int maxOR{};
		for (int i{1}; i <= N; i++)
		{
			cin >> arr[i];
			maxOR |= arr[i];
		}
		int ans = minLength_maxOR(arr, maxOR);
		cout << ans;
	}
	return 0;
}