#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

/*
Count number of bits to be flipped to convert A to B

Solution:
---------
Calculate (A XOR B)
Since (0 XOR 1) = 1 and (1 XOR 0) = 1
while (0 XOR 0) = 0 and (1 XOR 1) = 0
So calculating the number of set bits (1's) in (A XOR B) will give us the number of mis-matching bits in A and B, which needs to be flipped
*/
int Number_of_bits_to_flip(ll A, ll B) // Counts different bits
{
	return __builtin_popcount(A ^ B);
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
	Fedor is the (M + 1)-th player of the game.
	He assume that two players can become friends if their armies differ in at most K types of soldiers
	(in other words, binary representations of the corresponding numbers differ in at most K bits).
	Help Fedor and count how many players can become his friends.
	*/
	int N, M, K;
	cin >> N >> M >> K;
	int arr[1010] = {0};
	for (int i{1}; i <= M + 1; i++)
		cin >> arr[i];

	int cnt{};
	for (int i{1}; i <= M; i++)
	{
		if (Number_of_bits_to_flip(arr[i], arr[M + 1]) <= K)
			cnt++;
	}
	cout << cnt;

	return 0;
}