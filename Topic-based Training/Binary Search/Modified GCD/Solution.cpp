#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

template <typename T>
vector<T> factorize(T N)
{
	vector<T> factors;
	for (ll d = 1; d * d <= N; d++)
	{
		if (N % d == 0)
		{
			ll f1 = d;
			ll f2 = N / d;
			factors.push_back(f1);
			if (f2 != f1)
				factors.push_back(f2);
		}
	}
	sort(factors.begin(), factors.end());
	return factors;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
	int t = 1;
	ll N, A, B, Q;
	// cin >> t;
	while (t--)
	{
		cin >> A >> B;
		vector<ll> divisorsA, divisorsB;
		divisorsA = factorize(A);
		divisorsB = factorize(B);
		vector<ll> commonDivisors;
		set_intersection(divisorsA.begin(), divisorsA.end(), divisorsB.begin(), divisorsB.end(), inserter(commonDivisors, commonDivisors.begin()));
		sort(commonDivisors.begin(), commonDivisors.end());
		cin >> Q;
		int L, R, idx;
		while (Q--)
		{
			cin >> L >> R;
			idx = upper_bound(commonDivisors.begin(), commonDivisors.end(), R) - commonDivisors.begin();
			idx--;
			if (idx == -1 || commonDivisors[idx] < L)
				cout << -1 << endl;
			else
				cout << commonDivisors[idx] << endl;
		}
	}
	return 0;
}