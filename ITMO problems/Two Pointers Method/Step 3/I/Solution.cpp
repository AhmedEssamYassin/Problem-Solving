#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct SubsetQueue
{
	int s;
	vector<int> leftVals, rightVals;
	vector<bitset<1001>> leftDP, rightDP;

	SubsetQueue(int _s) : s(_s)
	{
		bitset<1001> bL, bR;
		bL.set(s);
		bR.set(0);
		leftDP.push_back(bL);
		rightDP.push_back(bR);
	}
	void pushLeft(int x)
	{
		leftVals.push_back(x);
		leftDP.push_back(leftDP.back() | (leftDP.back() >> x));
	}
	void push(int x)
	{
		rightVals.push_back(x);
		rightDP.push_back(rightDP.back() | (rightDP.back() << x));
	}
	void pop()
	{
		if (leftVals.empty())
		{
			while (!rightVals.empty())
			{
				pushLeft(rightVals.back());
				rightVals.pop_back();
				rightDP.pop_back();
			}
		}
		leftVals.pop_back();
		leftDP.pop_back();
	}
	bool good() const
	{
		return (leftDP.back() & rightDP.back()).any();
	}
};

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif
	int t = 1;
	// cin >> t;
	while (t--)
	{
		int n, s;
		cin >> n >> s;
		vector<int> a(n);
		for (int i{}; i < n; i++)
			cin >> a[i];

		SubsetQueue q(s);
		int L = 0, ans = INT_MAX;
		for (int R{}; R < n; R++)
		{
			q.push(a[R]);
			while (q.good())
			{
				ans = min(ans, R - L + 1);
				q.pop();
				L++;
			}
		}
		cout << (ans == INT_MAX ? -1 : ans);
	}
	return 0;
}
