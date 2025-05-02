#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

short int arr[16];
short int N_rotations;
bool DP(int Current, int pos)
{
	if (pos == N_rotations)
		return (Current % 360 == 0);

	bool Choice_1 = DP(Current + arr[pos], pos + 1);
	bool Choice_2 = DP(Current - arr[pos], pos + 1);
	return (Choice_1 || Choice_2);
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
	At each Angle, We have two choices:
	1. To rotate right (means to ADD this angle)
	2. To rotate left (means to SUBTRACT this angle)
	If the final value is 0, 360, 720 and so on (multiples of 360), that means we came back to 360 again
	*/
	cin >> N_rotations;

	for (int i{}; i < N_rotations; i++)
		cin >> arr[i];

	if (DP(0, 0))
		cout << "YES";
	else
		cout << "NO";
	return 0;
}