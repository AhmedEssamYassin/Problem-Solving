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
	ll N;
	// cin >> t;
	while (t--)
	{
		cin >> N;
		vector<string> chatNames(N);
		stack<string> chatList;
		set<string> st;
		for (int i{}; i < N; i++)
			cin >> chatNames[i];
		for (int i = N - 1; i >= 0; i--)
		{
			if (!st.count(chatNames[i])) // first time to appear
				chatList.push(chatNames[i]);
			st.insert(chatNames[i]);
		}
		vector<string> finalList;
		while (!chatList.empty())
		{
			finalList.push_back(chatList.top());
			chatList.pop();
		}
		reverse(finalList.begin(), finalList.end());
		for (const string &chatName : finalList)
			cout << chatName << endl;
	}
	return 0;
}