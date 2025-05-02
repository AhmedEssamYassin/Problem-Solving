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
	ll N, K;
	cin >> t;
	while (t--)
	{
		cin >> N >> K;
		int *arr = new int[N];
		for (int i{}; i < N; i++)
			cin >> arr[i];

		int cnt{}, len{1};
		int L{}, R{1};
		// 22 12 16 4 3 22 12
		// L  L  L  R
		while (R < N)
		{
			if (2 * arr[R] > arr[L])
				len++, L++, R++;
			else
			{
				/*
				Suppose we have a strictly increasing sub_array {a, b, c, d, e, ...} of length len
				This sub_array can form (len - K) valid sub_arrays
				For a 1-based position pos to be a valid starting index to a sub_array, (len - pos + 1) >= K + 1
				(len - pos) >= K --> pos <= (len - K)
				So the max pos we can get starting from 1 is (len - K) which is also the number of valid sub_arrays
				e.g. suppose K = 2, {a, b, c}, {b, c, d}, {c, d, e} ... (len - K) sub_arrays
				*/
				if (len >= K + 1)
					cnt += (len - K);

				L = R, ++R, len = 1;
			}
		}
		if (len >= K + 1)
			cnt += (len - K);
		cout << cnt << endl;

		delete[] arr;
	}
	return 0;
}