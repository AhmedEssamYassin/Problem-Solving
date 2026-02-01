#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif

	float Number;
	cin >> Number;
	if (int(Number) == Number)
		cout << "int " << int(Number);
	else
		cout << "float " << int(Number) << " " << (Number - int(Number));

	return 0;
}