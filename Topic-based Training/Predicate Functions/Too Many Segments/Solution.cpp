#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct segment
{
	int li, ri, pos;
	bool operator<(const segment &other) const
	{
		return ri != other.ri ? ri < other.ri : pos < other.pos;
	}
};

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
	int t = 1;
	ll N, k;
	// cin >> t;
	while (t--)
	{
		cin >> N >> k;
		vector<vector<segment>> vc(2e5 + 1);
		for (int i{1}; i <= N; i++)
		{
			int li, ri;
			cin >> li >> ri;
			vc[li].push_back({li, ri, i});
		}
		vector<int> ans;
		set<segment> st;
		for (int i = 1; i <= 2e5; i++)
		{
			for (const auto &seg : vc[i])
				st.insert(seg);
			while (!st.empty() && st.begin()->ri < i)
				st.erase(st.begin());
			while (st.size() > k)
			{
				segment it = *st.rbegin();
				ans.push_back(it.pos);
				st.erase(it);
			}
		}
		cout << ans.size() << endl;
		for (const int &i : ans)
			cout << i << " ";
	}
	return 0;
}