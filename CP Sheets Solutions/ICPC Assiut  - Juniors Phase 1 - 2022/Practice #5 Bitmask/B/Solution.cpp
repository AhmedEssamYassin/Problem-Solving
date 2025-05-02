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
	- In the interval [A, B], How many numbers contain EXACTLY 1 zero (such as 101, 1101, but NOT 1010)

	Each number with EXACTLY one zero can be obtained by taking the number without any zeros (e.g. 63 = 111111) and subtracting some power of two,
	e.g. 63 - 16 = 111111 - 010000 = 101111
	Subtracting a power of two changes one digit from '1' to '0' and this is what we want.
	But how can we iterate over numbers without any zeros?
	- It turns out that each of them is of form (2^x - 1) for some x (you can check that it's true for 63)

	What should we do to solve this problem?
	Iterate over possible values of x to get all possible (2^x - 1) — numbers without any zeros.
	There are at most log(10^18) values to consider because we don't care about numbers much larger than 10^18.
	For each (2^x - 1) you should iterate over powers of two and try subtracting each of them.
	Now you have candidates for numbers with exactly one zero.
	For each of them check if it is in the given interval, and if so, count it!
	*/
	ll A, B;
	cin >> A >> B;
	int cnt{};
	for (int i{}; i <= 63; i++) //(2^63) - 1 > 10^18, So, it's sufficient
	{
		ll val = (1LL << i) - 1;
		for (int j{}; j < i - 1; j++) // We don't count the numbers with one zero as a leading zero!(such as 0111)
		{
			ll singleZeroBitmask = val - (1LL << j);
			if (singleZeroBitmask >= A && singleZeroBitmask <= B)
				cnt++;
		}
	}
	cout << cnt;
	return 0;
}