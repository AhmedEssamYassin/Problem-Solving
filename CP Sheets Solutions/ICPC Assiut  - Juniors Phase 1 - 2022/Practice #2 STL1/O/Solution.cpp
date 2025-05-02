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
		string cmd, path;
		deque<string> curPath;
		curPath.push_back("/");
		for (int i{}; i < N; i++)
		{
			cin >> cmd;
			if (cmd == "pwd")
			{
				for (int j{}; j < curPath.size(); j++)
					cout << curPath[j] << (curPath[j].back() != '/' ? "/" : "");
				cout << endl;
			}
			else // cd : change directory
			{
				cin >> path;
				string temp("");
				if (path.front() == '/') // Absolute path
				{
					curPath.clear();
					curPath.push_back("/");
				}
				for (int j{}; j < path.size(); j++)
				{
					if (path[j] != '/')
						temp += path[j];
					if (path[j] == '/' || j == path.length() - 1)
					{
						if (temp == "..")
							curPath.pop_back();
						else
						{
							if (temp == "")
								continue;
							curPath.push_back(temp);
						}
						temp = "";
					}
				}
			}
		}
	}
	return 0;
}