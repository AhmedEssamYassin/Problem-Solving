#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

const ll L1D_CACHE_SIZE = 32768;

// Optimized segmented sieve for range [L, R]
pair<vector<ll>, ll> getPrimesInRange(ll L, ll R)
{
    if (L > R)
        return {{}, 0};
    if (R < 2)
        return {{}, 0};

    vector<ll> result;

    // Handle special case for 2
    if (L <= 2 && R >= 2)
    {
        result.push_back(2);
    }

    // Make L odd if it's even (we only check odd numbers)
    if (L % 2 == 0)
        L++;

    ll sqrt_R = sqrtl(R);
    ll segment_size = max(sqrt_R, L1D_CACHE_SIZE);

    // Generate base primes up to sqrt(R) using simple sieve
    vector<bool> isPrime(sqrt_R + 1, true);
    isPrime[0] = isPrime[1] = false;

    // Only sieve odd numbers for base primes
    for (ll i = 3; i * i <= sqrt_R; i += 2)
    {
        if (isPrime[i])
        {
            for (ll j = i * i; j <= sqrt_R; j += 2 * i)
                isPrime[j] = false;
        }
    }

    // Collect odd primes >= 3 for sieving
    vector<ll> primes;
    vector<ll> multiples;

    for (ll low = L; low <= R; low += segment_size)
    {
        ll high = min(low + segment_size - 1, R);

        // Use char array for better cache performance
        vector<char> sieve((high - low) / 2 + 1, true);

        // Add new primes to our sieving list as needed
        for (ll p = 3; p * p <= high; p += 2)
        {
            if (isPrime[p] && (primes.empty() || p > primes.back()))
            {
                primes.push_back(p);

                // Find first odd multiple of p in range [low, high]
                ll first_multiple = max(p * p, ((low + p - 1) / p) * p);
                if (first_multiple % 2 == 0)
                    first_multiple += p;

                multiples.push_back((first_multiple - low) / 2);
            }
        }

        // Sieve current segment (only odd numbers)
        for (size_t i = 0; i < primes.size(); i++)
        {
            ll p = primes[i];
            ll j = multiples[i];
            ll segment_limit = (high - low) / 2;

            // Mark multiples of p in current segment
            while (j <= segment_limit)
            {
                sieve[j] = false;
                j += p; // Skip by p (since we're dealing with odd numbers only)
            }

            // Update multiple for next segment
            multiples[i] = j - (segment_limit + 1);
        }

        // Collect primes from current segment
        for (ll i = 0; i <= (high - low) / 2; i++)
        {
            if (sieve[i])
            {
                ll candidate = low + 2 * i;
                if (candidate >= L && candidate <= R && candidate > 1)
                    result.push_back(candidate);
            }
        }
    }

    return {result, (ll)result.size()};
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
    cin >> t;
    while (t--)
    {
        int L, R;
        cin >> L >> R;
        auto [primes, cnt] = getPrimesInRange(L, R);
        for (ll &p : primes)
            cout << p << endl;
        cout << endl;
    }
    return 0;
}