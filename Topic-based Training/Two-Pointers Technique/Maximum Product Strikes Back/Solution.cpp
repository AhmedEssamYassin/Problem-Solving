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
	int t = 1;
	ll N, K, a, b;
	cin >> t;
	while (t--)
	{
		cin >> N;
		vector<ll> a(N);
		for (int i{}; i < N; i++)
			cin >> a[i];

		ll ans = 0;
		ll beginning = N, end = 0;
		for (int i = 0, L = -1; i <= N; i++) // L = -1, to start at index 0 when first zero is found
		{
			if (i == N || a[i] == 0)
			{
				bool neg = false; // Assumed to be initially a positive sign
				ll left = -1, right = -1;
				ll cnt = 0; // count 2's on the while segment
				ll cl = 0;	// count 2's to delete from the left
				ll cr = 0;	// count 2's to delete from the right

				// Traversing the segment between two zeros, the beginning and one zero or one zero and the end
				for (int j = L + 1; j < i; j++)
				{
					neg ^= (a[j] < 0); // Every negative we encounter the sign flips
					if (a[j] < 0)
					{
						right = j; // Maintaining the index of the rightmost negative number
						cr = 0;
					}

					if (abs(a[j]) == 2)
					{
						cnt++, cr++;
						if (left == -1) // Not seeing a negative number yet
							cl++;
					}

					if (a[j] < 0 && left == -1)
						left = j; // Maintaining the index of the first negative number
				}
				// If it's positive, we don't need to remove anything
				// So we only process when it's negative
				if (neg)
				{
					if (cl < cr) // Number of 2's to delete from the left is less than that from the right
					{
						right = i;
						cnt -= cl;
					}
					else
					{
						left = L;
						cnt -= cr;
					}
				}
				else // Positive
					left = L, right = i;

				if (cnt > ans)
				{
					ans = cnt;
					beginning = left + 1, end = N - right;
				}
				L = i; // To process the next segment
			}
		}
		cout << beginning << " " << end << endl;
	}
	return 0;
}