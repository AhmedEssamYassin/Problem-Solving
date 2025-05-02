#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct participant
{
	string name;
	ll rate;
	participant(string name, ll rate) : name(name), rate(rate) {}
};

bool predFunc(const participant &P1, const participant &P2)
{
	// If both participants have the same rate, prioritize the one with lexicographically smaller name
	if (P1.rate == P2.rate)
		return P1.name < P2.name;

	// Otherwise, prioritize the one with the greater rate
	return P1.rate > P2.rate;
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
	ll N;
	// cin >> t;
	while (t--)
	{
		cin >> N;
		map<string, ll> mp;
		for (int i{}; i < N; i++)
		{
			ll M;
			cin >> M;
			ll prevRate = 1000;
			for (int j{}; j < M; j++)
			{
				string name;
				cin >> name;
				if (j == 0)
					mp[name] += prevRate;
				else // 90% is 0.9
					mp[name] += (prevRate = round((9 * prevRate) / 10.0));
			}
		}
		vector<participant> vc;
		for (const auto &[name, rate] : mp)
			vc.push_back({name, rate});
		sort(vc.begin(), vc.end(), predFunc);
		cout << vc.front().name << endl;
		for (const auto &[name, rate] : vc)
			cout << name << " " << rate << endl;
	}
	return 0;
}