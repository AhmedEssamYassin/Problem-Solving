#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

/*
Count numbers up to (N) having the Kth bit set
--------------------------------------------------
1. Right-shift N, K+1 times followed by left-shifting the result K times
   which gives the count of numbers satisfying the given condition till the nearest Power_of_2 less than N

2. Now, check if the Kth bit is set in N or NOT

3. If the Kth bit is set in N, then add the count of numbers from the nearest Power_of_2 less than N to the answer
   Then add 1 to count N itself

Let's figure out why this method works!
consider a range of numbers until N = 22 and we want to count how many number with bit K = 2 set
0000
0001
0010
0011
0100 ✓
0101 ✓
0110 ✓
0111 ✓
1000
1001
1010
1011
1100 ✓
1101 ✓
1110 ✓
1111 ✓
10000
10001
10010
10011
10100 ✓
10101 ✓
10110 ✓
.
.
.

We can observe that starting from 2² we have 2² numbers with Kth = 2 bit set and 2² NOT and this pattern keeps repeating
That means we need to know how many 2² are in N which is easily calculated using integer division res = (N / 2²)
but NOT ALL 2²'s are satisfying the condition, ONLY half of them, so we need to further divide by 2 (res / 2)
Now, res value is number of groups of numbers with Kth = 2 bit set, each group is 2²
That means total count of numbers with Kth = 2 bit set is res *= 2²
That totally reasons the first step in the algorithm:
res = (N >> (K + 1)) << K
which is equivalent to (N / 2^K / 2) * 2^K

What about an incomplete group of 2^K?
In our example, 22/2²/2 = 2 because integer division ONLY counts complete 2^K's existing in N
so we did NOT count the last group of 2²!
Now we need to check if this group satisfies our condition and if so, we add it the result

If the Kth bit is set in N itself, that means we are in a valid group
then add (N & ((1 << K) - 1))
10110
&
00011

Then add 1 to count N itself
*/

template <typename T>
int count_kth_set_bit(const T &N, const int &K)
{
	int res = (N >> (K + 1)) << K;

	if (N & (T(1) << K))
		res += (N & ((T(1) << K) - 1)) + 1; // Adds 1 to count N itself

	return res;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE

	/*
	You are given an array consisting of all integers from [L, R] inclusive.
	For example, if L = 2 and R = 5, the array would be {2, 3, 4, 5}
	What's the minimum number of elements you can delete to make the bitwise AND of the array NON-zero?

	Let's solve the complement problem:
	Find the largest subsequence of the array such that their bitwise AND is NON-zero.
	- Let X be the bitwise AND of the optimal subsequence.
	  Let N be (R - L + 1) which is number of elements in range [L, R]
	Since X != 0, AT LEAST one bit must be set in X
	- Let's iterate over that bit, call it b, and in each iteration, calculate the largest subsequence whose bitwise AND has that bit set.
	- For the b-th bit to be set in the final answer, EVERY element in the chosen subsequence must have that bit set.
	- Since choosing every element with the b-th bit set is a valid subsequence,
	  this implies that the answer for the b-th bit is the number of elements that have the b-th bit set.
	  Thus, the answer to the final problem is N − max_cnt, where cnt is the number of elements who have the b-th bit set.

	Note:
	It doesn't matter if the final answer contains more than one set bit, it's still covered in at least one of the cases
	and the bitwise AND will still be NON-zero.

	Example:
	L = 5, R = 25
	00000101
	00000110
	00000111
	00001000
	00001001
	00001010
	00001011
	00001100
	00001101
	00001110
	00001111
	00010000
	00010001
	00010010
	00010011
	00010100
	00010101
	00010110
	00010111
	00011000
	00011001

	It's obvious that if we knew that Kth bit is most frequent in ALL numbers in range,
	then deleting ALL other elements will Yield a NON-zero AND with minimum number of elements deleted
	*/
	int t, L, R;
	cin >> t;
	while (t--)
	{
		cin >> L >> R;
		int Ans = R - L + 1; // Number of element in range [L, R]
		int N = R - L + 1;
		for (int ith_bit{}; ith_bit < 31; ith_bit++) // Iterate over ALL bits
		{
			int X = count_kth_set_bit(L - 1, ith_bit);
			int Y = count_kth_set_bit(R, ith_bit);
			// (Y - X) evaluates the number of elements with Kth bit set in range [L, R]
			Ans = min(Ans, N - (Y - X));
		}
		cout << Ans << endl;
	}
	return 0;
}