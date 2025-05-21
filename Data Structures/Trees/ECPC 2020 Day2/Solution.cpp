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
	freopen("tree.in", "r", stdin);
	int t = 1;
	ll N;
	// cin >> t;
	while (t--)
	{
		string root;
		cin >> root;
		cin >> N;
		map<string, vector<string>> Tree;
		for (int i{}; i < N; i++)
		{
			string parent, child;
			cin >> parent >> child;
			Tree[parent].push_back(child);
		}
		int cntDirectories{}, cntFiles{};
		auto dfs = [&](const string &node, const string &prefix, bool isLast, const string &root, auto &self) -> void
		{
			if (node != root)
			{
				cout << prefix;
				cout << (isLast ? "|__ " : "|-- ");
			}
			cout << node << "\n";

			if (Tree.find(node) == Tree.end())
			{
				cntFiles++;
				return;
			}
			cntDirectories++;

			sort(Tree[node].begin(), Tree[node].end());
			int sz = Tree[node].size();
			for (int i = 0; i < sz; ++i)
			{
				string newPrefix = prefix;
				if (node != root)
					newPrefix += (isLast ? "   " : "|   ");
				self(Tree[node][i], newPrefix, i == sz - 1, root, self);
			}
		};
		dfs(root, "", true, root, dfs);
		cout << endl;
		cout << cntDirectories << " directories, " << cntFiles << " files" << endl;
	}
	return 0;
}