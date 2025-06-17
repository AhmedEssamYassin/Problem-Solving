#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

vector<int> Prime;
vector<int> LPF, phi;
void precomputePhi(int N)
{
    LPF.assign(N + 1, 0);
    phi.resize(N + 1);
    phi[1] = 1;
    for (int i = 2; i <= N; i++)
    {
        if (LPF[i] == 0)
        {
            LPF[i] = i;
            phi[i] = i - 1;
            Prime.push_back(i);
        }
        else
        {
            // Calculating phi
            if (LPF[i] == LPF[i / LPF[i]])
                phi[i] = phi[i / LPF[i]] * LPF[i];
            else
                phi[i] = phi[i / LPF[i]] * (LPF[i] - 1);
        }
        for (int j = 0; j < (int)Prime.size() && Prime[j] <= LPF[i] && i * Prime[j] <= N; j++)
            LPF[i * Prime[j]] = Prime[j];
    }
}

static int autoCall = (precomputePhi(1000000), 0);

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
    cin >> t;
    while (t--)
    {
        cin >> N;
        cout << phi[N] << endl;
    }
    return 0;
}
