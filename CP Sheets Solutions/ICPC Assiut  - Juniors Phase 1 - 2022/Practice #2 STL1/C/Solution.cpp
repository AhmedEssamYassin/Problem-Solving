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
	ll N, Q;
	// cin >> t;
	while (t--)
	{
		cin >> N;
		stack<string> stk;
		string tag;
		bool Valid = true; // Assuming it's valid until some tag violates it
		for (int i{}; i < N; i++)
		{
			cin >> tag;
			if (i == 0 && tag != "Header") //"Header" should be the first tag
				Valid = false;			   // Don't break until you parse all input, it's a bad practice

			if (i == N - 1 && tag != "EndHeader") //"EndHeader" should be the last tag
				Valid = false;					  // Don't break until you parse all input, it's a bad practice

			if ((i > 0 && tag == "Header") || (i < N - 1 && tag == "EndHeader"))
				Valid = false;

			if (tag.substr(0, 3) == "End") // It's a closing tag
			{
				if (tag == "End" + stk.top())
					stk.pop();
				else
					Valid = false;
			}
			else // It's an opening of a tag
				stk.push(tag);
		}
		if (Valid && stk.empty()) // All tags have been closed
			cout << "ACC";
		else
			cout << "WA";
	}
	return 0;
}