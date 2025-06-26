#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

bitset<1000001> isPrime;
// Sieving to sqrt(N) ONLY for marking primes (NOT generating them)
void SieveOfEratosthenes(int N)
{
    isPrime.set();               // Initially Assuming all numbers to be primes
    isPrime[0] = isPrime[1] = 0; // 0 and 1 are NOT primes
    for (ll i = 2; i * i <= N; i++)
    {
        if (isPrime[i])
        {
            for (ll j = i * i; j <= N; j += i)
                isPrime[j] = false;
        }
    }
}
static int autoCall = (SieveOfEratosthenes(1000000), 0);

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
    }
    return 0;
}
