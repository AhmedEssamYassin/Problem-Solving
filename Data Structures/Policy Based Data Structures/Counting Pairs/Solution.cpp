#include <ext/pb_ds/assoc_container.hpp> // Common file
#include <ext/pb_ds/tree_policy.hpp>
#include <functional> // for less
#include <iostream>
using namespace __gnu_pbds;
using namespace std;
template <typename T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
template <typename T>
using ordered_multiset = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
#include <bits/stdc++.h>
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
	ll N;
	cin >> t;
	while (t--)
	{
		ll x, y;
		cin >> N >> x >> y;
		vector<ll> a(N);
		for (int i{}; i < N; i++)
			cin >> a[i];
		ll sum = accumulate(a.begin(), a.end(), 0LL);
		ordered_set<pair<ll, ll>> OS;
		ll cnt{};
		for (int i{}; i < N; i++)
		{
			// x <= sum - a[i] - a[j] <= y
			auto it1 = OS.order_of_key({sum - y - a[i], -2});
			auto it2 = OS.order_of_key({sum - x - a[i], N + 1});
			cnt += it2 - it1;
			OS.insert({a[i], i});
		}
		cout << cnt << endl;
	}
	return 0;
}