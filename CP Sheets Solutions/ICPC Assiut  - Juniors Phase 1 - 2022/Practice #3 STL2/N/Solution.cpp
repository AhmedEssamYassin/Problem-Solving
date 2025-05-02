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
		stack<pair<string, char>> firstStack;
		stack<pair<string, char>> secondStack;
		map<string, string> variableMap;

		ll firstCmdCount;
		cin >> firstCmdCount;
		int idCounter = 1;

		while (firstCmdCount--)
		{
			string command;
			cin >> command;

			if (command == "define")
			{
				string variable;
				cin >> variable;
				variableMap[variable] = idCounter;
				idCounter++;
				firstStack.push({variableMap[variable], 'd'});
			}
			else if (command == "print")
			{
				string variable;
				cin >> variable;
				firstStack.push({variableMap[variable], 'p'});
			}
			else if (command == "read")
			{
				string variable;
				cin >> variable;
				firstStack.push({variableMap[variable], 'r'});
			}
			else
			{
				string var = "", result = "";
				for (int j = 0; j < command.size(); j++)
				{
					if (isalpha(command[j]))
						var += command[j];
					else
					{
						if (!var.empty())
							result += variableMap[var];
						result += command[j];
						var = "";
					}
				}
				if (!var.empty())
					result += variableMap[var];
				firstStack.push({result, 'p'});
			}
		}

		ll secondCmdCount;
		cin >> secondCmdCount;

		idCounter = 1;

		while (secondCmdCount--)
		{
			string command;
			cin >> command;

			if (command == "define")
			{
				string variable;
				cin >> variable;
				variableMap[variable] = idCounter;
				idCounter++;
				secondStack.push({variableMap[variable], 'd'});
			}
			else if (command == "print")
			{
				string variable;
				cin >> variable;
				secondStack.push({variableMap[variable], 'p'});
			}
			else if (command == "read")
			{
				string variable;
				cin >> variable;
				secondStack.push({variableMap[variable], 'r'});
			}
			else
			{
				string var = "", result = "";
				for (int j = 0; j < command.size(); j++)
				{
					if (isalpha(command[j]))
						var += command[j];
					else
					{
						if (!var.empty())
							result += variableMap[var];
						result += command[j];
						var = "";
					}
				}
				if (!var.empty())
					result += variableMap[var];
				secondStack.push({result, 'p'});
			}
		}

		while (!secondStack.empty() && !firstStack.empty())
		{
			if (secondStack.top() != firstStack.top() || secondStack.size() != firstStack.size())
				return cout << "NO", 0;
			secondStack.pop();
			firstStack.pop();
		}

		cout << "YES";
	}
	return 0;
}