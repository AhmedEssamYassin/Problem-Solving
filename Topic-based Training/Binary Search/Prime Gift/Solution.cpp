#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

constexpr ll maxVal = 1LL * 1000 * 1000 * 1000 * 1000 * 1000 * 1000;
void preCompute(const vector<ll> &in, vector<ll> &out, ll cur = 1, int k = 0, ll bound = maxVal)
{
	// cout << cur << endl;
	if (k >= in.size())
	{
		out.push_back(cur);
		return;
	}
	while (bound > 0)
	{
		preCompute(in, out, cur, k + 1, bound);
		cur *= in[k];
		bound /= in[k];
	}
}

ll countLess(ll mid, const vector<ll> &DA, const vector<ll> &DB)
{
	ll res = 0;
	int j = 0;
	/*
	DA[i] * DB[j] = x
	DA[i] * DB[j] <= mid, that means there is j numbers now less or equal to mid
	because DB is sorted in ascending order, so every number with index < j, will also make this condition hold

	Just to avoid overflow, it's better to write DB[j] <= mid / DA[i]
	*/
	for (int i{}; i < DA.size(); i++)
	{
		while (j < DB.size() && DB[j] <= mid / DA[i])
			j++;
		res += j;
	}
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
	int t = 1;
	ll N, K, x;
	// cin >> t;
	while (t--)
	{
		cin >> N;
		vector<ll> A, B, DA, DB;
		for (int i{}; i < N; i++)
		{
			cin >> x;
			if (i & 1)
				A.push_back(x);
			else
				B.push_back(x);
		}
		cin >> K;
		preCompute(A, DA);
		preCompute(B, DB);
		sort(DA.rbegin(), DA.rend());
		sort(DB.begin(), DB.end());
		ll L{}, R = maxVal + 1, ans = -1;
		while (L <= R)
		{
			ll mid = ((L + R) >> 1);
			ll cntLess = countLess(mid, DA, DB);
			if (cntLess < K)
				L = mid + 1;
			else
				ans = mid, R = mid - 1;
		}
		cout << ans;
	}
	return 0;
}