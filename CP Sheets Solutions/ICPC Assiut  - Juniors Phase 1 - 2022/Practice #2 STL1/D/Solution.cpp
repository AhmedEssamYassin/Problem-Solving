#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

char discard(stack<char> &M, stack<char> &A, stack<char> &R, int turn) // 0 mohamed ::: 1 rady ::: 2 Ali
{
	if (turn == 0)
	{
		if (M.empty())
			return 'M';
		char C = M.top();
		M.pop();
		if (C == 'r')
			return discard(M, A, R, 1);
		else if (C == 'a')
			return discard(M, A, R, 2);
		else
			return discard(M, A, R, 0);
	}
	else if (turn == 1)
	{
		if (R.empty())
			return 'R';

		char C = R.top();
		R.pop();
		if (C == 'r')
			return discard(M, A, R, 1);
		else if (C == 'a')
			return discard(M, A, R, 2);
		else
			return discard(M, A, R, 0);
	}
	else
	{
		if (A.empty())
			return 'A';

		char C = A.top();
		A.pop();
		if (C == 'r')
			return discard(M, A, R, 1);
		else if (C == 'a')
			return discard(M, A, R, 2);
		else
			return discard(M, A, R, 0);
	}
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
	int N;
	// cin >> t;
	while (t--)
	{
		string M, A, R;
		cin >> M >> A >> R;
		stack<char> Mohamed, Ali, Rady;

		for (int i = M.length() - 1; i >= 0; i--)
			Mohamed.push(M[i]);

		for (int i = A.length() - 1; i >= 0; i--)
			Ali.push(A[i]);

		for (int i = R.length() - 1; i >= 0; i--)
			Rady.push(R[i]);

		cout << discard(Mohamed, Ali, Rady, 0);
	}
	return 0;
}