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
#endif
	int t;
	ll N;
	cin >> t;
	while (t--)
	{
		cin >> N;
		vector<int> a(N);
		for (int &element : a)
			cin >> element;
		int L{}, R{N - 1};
		bool dir = false;
		while (L <= R)
		{
			if (dir = !dir)
				cout << a[L++] << " ";
			else
				cout << a[R--] << " ";
		}
		cout << endl;
	}
	return 0;
}