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
	First, note that bitwise operations do NOT result in any interaction between different positions,
	so we can just look at what happens to the bits in each position. And there are just four possibilities:
	0 → 0, 1 → 1. Equivalent to any of |0, &1 or ^0
	0 → 0, 1 → 0. Equivalent to &0
	0 → 1, 1 → 0. Equivalent to ^1
	0 → 1, 1 → 1. Equivalent to |1

	Thus, by following what happens at each bit position,
	we can decide to apply one each of &, | and ^ operations with the correct arguments and get any desired result.
	In fact, we need ONLY 2 instead of 3 operations because &0 is equivalent to |1 followed by ^1.
	(0 & 0) = 0, (1 & 0) = 0
	So, anything ANDed with 0 will yield a zero
	We know that (1 ^ 1) = 0, but (0 ^ 1) = 1
	So we need to convert that bit to 1, then apply XOR 1 which guarantees that bit to change to 0
	((0 | 1) ^ 1) = 0, ((1 | 1) ^ 1) = 0

	Combining the functions for all positions gives us the answer
	Suppose the input is X
	My K-line program should do EXACTLY the same as Petya's program would do to the input X

	The problem is, we don't know if that program will work on 0 or NON-zero inputs
	e.g., If input is ALL zeros and there are several AND operations, they wouldn't change bits at all
	So we need to assume one possible input to be ALL zeros and another to be ALL ones
	Then, accumulate all operations on both these assumptions
	*/
	int N, X;
	char bitwiseOp;
	cin >> N;

	int zeros = 0;
	int ones = 1023; // 0b1111111111

	while (N--)
	{
		cin >> bitwiseOp >> X;
		if (bitwiseOp == '|')
			zeros |= X, ones |= X;

		else if (bitwiseOp == '&')
			zeros &= X, ones &= X;

		else if (bitwiseOp == '^')
			zeros ^= X, ones ^= X;
	}
	///////////////////////////////

	int OR = 0, XOR = 0;

	for (int i = 0; i < 10; i++, zeros >>= 1, ones >>= 1)
	{
		/*
		zeros: 010011010
		ones : 011000111
		0 → 0, 1 → 1. Equivalent to any of |0, &1 or ^0
		0 → 0, 1 → 0. Equivalent to &0
		0 → 1, 1 → 0. Equivalent to ^1
		0 → 1, 1 → 1. Equivalent to |1
		*/
		if ((zeros & 1) == 0 && (ones & 1) == 1)
			OR |= (0 << i), XOR ^= (0 << i); // It's like doing NOTHING
		else if ((zeros & 1) == 0 && (ones & 1) == 0)
			OR |= (1 << i), XOR ^= (1 << i); // It's like &0
		else if ((zeros & 1) == 1 && (ones & 1) == 0)
			XOR ^= (1 << i);
		else if ((zeros & 1) == 1 && (ones & 1) == 1)
			OR |= (1 << i);
	}
	cout << "2\n";
	cout << "| " << OR << endl;
	cout << "^ " << XOR << endl;
	return 0;
}