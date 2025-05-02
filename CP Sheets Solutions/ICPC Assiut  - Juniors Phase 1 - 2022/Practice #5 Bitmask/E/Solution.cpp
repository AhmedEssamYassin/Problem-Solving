#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

template <typename T>
bool Check_ith_bit_set(T &N, uint16_t i) // 0-based
{
	// 11100101(0)11 & 00000000(1)00 = 00000000000 (it's a cleared bit)
	return (N & (1 << i));
	// return ((N >> i) & 1) == 1; // 110100, i = 4  -->  110 & 1 = 0
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
	In Your faculty, there is N student's some of them can work with each other, but NOT all of them can work with each other.
	Your job is to create a team from them such that ALL of its members can work with each other

	Note:
	If there are two students a and b, it is possible that student a can work with student b
	But is NOT guaranteed that student b can work with student a

	Since N is small enough (N <= 20), we can generate ALL subsets
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
	short int N, jth;
	cin >> N;
	int playersBitmasks[21] = {0}; // 21 bitmasks are sufficient for (N <= 20) players

	for (int i{}; i < N; i++)
	{
		for (int j{}; j < N; j++)
		{
			cin >> jth;
			playersBitmasks[i] |= ((1 << j) * jth); // Build ALL bitmasks, each is reversed (i.e., 1001101 will be 1011001)
		}
	}

	vector<pair<int, int>> Team, ans;
	for (int Bitmask = 0; Bitmask < (1 << N); Bitmask++) // 2^N possible Subsets (teams)
	{
		for (int ith_bit = 0; ith_bit < N; ith_bit++) // To include elements of this Subset
		{
			// If ith_bit is set, we include the ith player in this subset (team)
			if (Bitmask & (1 << ith_bit))
				Team.push_back({ith_bit + 1, playersBitmasks[ith_bit]}); // Storing pairs of {1-based Player order,Player's Bitmask}
		}

		int val = 0;
		if (!Team.empty())
			val |= Team[0].second;

		for (int i{1}; i < Team.size(); i++)
			val &= Team[i].second;

		bool valid_subset = true;
		// ALL remaining set bits represent the players who can play with each other
		// If the Players in this subset are marked as "Can play with each other" in the bitmask, then it's a valid subset
		for (int i{}; i < Team.size(); i++)
		{
			if (Check_ith_bit_set(val, Team[i].first - 1) == 0)
			{
				valid_subset = false;
				break;
			}
		}

		if (valid_subset && (Team.size() > ans.size()))
			ans = Team; // A candidate answer

		Team.clear();
	}

	cout << ans.size() << endl;
	for (int i{}; i < ans.size(); i++)
		cout << ans[i].first << " ";

	return 0;
}