#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

vector<int> primes;
vector<int> LPF, phi;
void precomputePhi(int N)
{
    LPF.assign(N + 1, 0);
    phi.resize(N + 1);
    phi[1] = 1;
    for (long long i = 2; i <= N; i++)
    {
        if (LPF[i] == 0)
        {
            LPF[i] = i;
            phi[i] = i - 1;
            primes.push_back(i);
        }
        else
        {
            // Calculating phi
            if (LPF[i] == LPF[i / LPF[i]])
                phi[i] = phi[i / LPF[i]] * LPF[i];
            else
                phi[i] = phi[i / LPF[i]] * (LPF[i] - 1);
        }
        for (long long j = 0; j < (int)primes.size() && primes[j] <= LPF[i] && i * primes[j] <= N; j++)
            LPF[i * primes[j]] = primes[j];
    }
}

static int autoCall = (precomputePhi(1000000), 0);

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifdef LOCAL
    freopen("input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif
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
