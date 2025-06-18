#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

const ll mod = 1e9 + 7;
#define double_size_t std::conditional_t<(mod > (1LL << 31)), __int128_t, long long>

inline ll add64(const ll &a, const ll &b)
{
    double_size_t res = double_size_t(a) + b;
    if (res >= mod)
        res -= mod;
    return res;
}

inline ll sub64(const ll &a, const ll &b)
{
    double_size_t res = double_size_t(a) - b;
    if (res < 0)
        res += mod;
    if (res >= mod)
        res -= mod;
    return res;
}

inline ll mult64(const ll &a, const ll &b)
{
    return double_size_t(a) * b % mod;
}

ll modPow(ll N, ll power, ll mod)
{
    if (N % mod == 0 || N == 0)
        return 0;
    if (N == 1 || power == 0)
        return 1;
    ll res{1};
    while (power)
    {
        if (power & 1)
            res = mult64(res, N);
        N = mult64(N, N);
        power >>= 1;
    }
    return res;
}

ll modInv(ll N, ll mod)
{
    return N <= 1 ? N : mod - (mod / N) * modInv(mod % N, mod) % mod;
}

// Generate sieve of Eratosthenes for prime numbers up to given limit
vector<ll> &generatePrimeTable(int upperLimit)
{
    ++upperLimit;
    const int SEGMENT_SIZE = 32768;
    static int processedLimit = 2;
    static vector<ll> primeNumbers = {2};
    static vector<ll> sieveArray(SEGMENT_SIZE + 1);

    if (processedLimit >= upperLimit)
        return primeNumbers;
    processedLimit = upperLimit;

    primeNumbers = {2};
    sieveArray.assign(SEGMENT_SIZE + 1, 0);
    const int halfLimit = upperLimit / 2;
    primeNumbers.reserve(int(upperLimit / log(upperLimit) * 1.1));

    vector<pair<ll, ll>> candidatePrimes;
    for (int i = 3; i <= SEGMENT_SIZE; i += 2)
    {
        if (!sieveArray[i])
        {
            candidatePrimes.emplace_back(i, i * i / 2);
            for (int j = i * i; j <= SEGMENT_SIZE; j += 2 * i)
                sieveArray[j] = 1;
        }
    }

    for (int leftBound = 1; leftBound <= halfLimit; leftBound += SEGMENT_SIZE)
    {
        array<bool, SEGMENT_SIZE> blockSieve{};
        for (auto &[prime, startIndex] : candidatePrimes)
        {
            for (int i = startIndex; i < SEGMENT_SIZE + leftBound; startIndex = (i += prime))
                blockSieve[i - leftBound] = 1;
        }
        for (int i = 0; i < min(SEGMENT_SIZE, halfLimit - leftBound); i++)
        {
            if (!blockSieve[i])
                primeNumbers.emplace_back((leftBound + i) * 2 + 1);
        }
    }
    return primeNumbers;
}

// Calculate sum of primes up to N using Lucy-Hedgehog algorithm
template <typename T>
pair<vector<T>, vector<T>> calculatePrimeSumWithFunction(ll maxNumber, function<T(ll)> summationFunction)
{
    /*
    Given N and a completely multiplicative function f with prefix sum function F,
    calculate sum_{p <= n} f(p) for all n = floor(N/d).

    This can compute sums of p^k or sums of p^k modulo m.

    Complexity: O(N^{3/4}/logN) time, O(N^{1/2}) space.
    */
    ll sqrtMaxNumber = sqrtl(maxNumber);
    auto &primeList = generatePrimeTable(sqrtMaxNumber);

    vector<T> lowSums(sqrtMaxNumber + 1), highSums(sqrtMaxNumber + 1);

    // Initialize with F(i) - 1 (subtract 1 since we don't want to count 1)
    for (int i = 1; i <= sqrtMaxNumber; i++)
        lowSums[i] = summationFunction(i) - 1;
    for (int i = 1; i <= sqrtMaxNumber; i++)
        highSums[i] = summationFunction(double(maxNumber) / i) - 1;

    // Lucy-Hedgehog sieve algorithm
    for (auto &&currentPrime : primeList)
    {
        ll primeSquared = currentPrime * currentPrime;
        if (primeSquared > maxNumber)
            break;

        ll rightBound = min(sqrtMaxNumber, maxNumber / primeSquared);
        ll middleBound = sqrtMaxNumber / currentPrime;
        T previousSum = lowSums[currentPrime - 1];
        T currentPrimeContribution = lowSums[currentPrime] - lowSums[currentPrime - 1];

        for (int i = 1; i <= middleBound; i++)
            highSums[i] -= currentPrimeContribution * (highSums[i * currentPrime] - previousSum);
        for (int i = middleBound + 1; i <= rightBound; i++)
            highSums[i] -= currentPrimeContribution * (lowSums[double(maxNumber) / (i * currentPrime)] - previousSum);
        for (int n = sqrtMaxNumber; n >= primeSquared; n--)
            lowSums[n] -= currentPrimeContribution * (lowSums[double(n) / currentPrime] - previousSum);
    }
    return {lowSums, highSums};
}

// Calculate count of primes up to N
template <typename T>
pair<vector<T>, vector<T>> calculatePrimeCount(ll maxNumber)
{
    auto identityFunction = [&](ll n) -> T
    { return n; };
    return calculatePrimeSumWithFunction<T>(maxNumber, identityFunction);
}

// Calculate sum of primes up to N
template <typename T>
pair<vector<T>, vector<T>> calculatePrimeSum(ll maxNumber)
{
    auto triangularFunction = [&](ll n) -> T
    {
        return (n & 1 ? T((n + 1) / 2) * T(n) : T(n / 2) * T(n + 1));
    };
    return calculatePrimeSumWithFunction<T>(maxNumber, triangularFunction);
}

// Main algorithm: Calculate sum of multiplicative function using Black's algorithm
template <typename T, typename FUNC>
T calculateMultiplicativeSum(ll maxNumber, FUNC multiplicativeFunction, vector<T> &lowSums, vector<T> &highSums)
{
    /*
    Given F(p^e) function and precomputed prime sums,
    calculate sum of multiplicative function over all positive integers up to N.

    Uses Black's algorithm from:
    http://baihacker.github.io/main/2020/The_prefix-sum_of_multiplicative_function_the_black_algorithm.html
    */
    ll sqrtMaxNumber = sqrtl(maxNumber);
    auto &primeList = generatePrimeTable(sqrtMaxNumber);

    auto getSumAtIndex = [&](ll divisor) -> T
    {
        return (divisor <= sqrtMaxNumber ? lowSums[divisor] : highSums[double(maxNumber) / divisor]);
    };

    T totalSum = T(1) + getSumAtIndex(maxNumber); // Include 1 and all primes

    // DFS to handle prime powers and their products
    // Parameters: (currentValue, primeIndex, exponent, f(currentValue), f(previousValue))
    auto depthFirstSearch = [&](auto self, ll currentValue, ll primeIndex, ll exponent,
                                T functionAtCurrent, T functionAtPrevious) -> void
    {
        T nextFunctionValue = functionAtPrevious * multiplicativeFunction(primeList[primeIndex], exponent + 1);

        // Add contribution from current prime power
        totalSum += nextFunctionValue;
        totalSum += functionAtCurrent * (getSumAtIndex(double(maxNumber) / currentValue) - getSumAtIndex(primeList[primeIndex]));

        ll upperLimit = sqrtl(double(maxNumber) / currentValue);

        // Recurse with higher powers of the same prime
        if (primeList[primeIndex] <= upperLimit)
            self(self, currentValue * primeList[primeIndex], primeIndex, exponent + 1, nextFunctionValue, functionAtPrevious);

        // Recurse with different primes
        for (int nextPrimeIndex = primeIndex + 1; nextPrimeIndex < (int)primeList.size(); nextPrimeIndex++)
        {
            if (primeList[nextPrimeIndex] > upperLimit)
                break;
            self(self, currentValue * primeList[nextPrimeIndex], nextPrimeIndex, 1,
                 functionAtCurrent * multiplicativeFunction(primeList[nextPrimeIndex], 1), functionAtCurrent);
        }
    };

    // Start DFS for each prime
    for (int i = 0; i < (int)primeList.size(); i++)
    {
        if (primeList[i] <= sqrtMaxNumber)
        {
            depthFirstSearch(depthFirstSearch, primeList[i], i, 1,
                             multiplicativeFunction(primeList[i], 1), 1);
        }
    }

    return totalSum;
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
        // Calculate prime counts and prime sums
        auto [primeCountLow, primeCountHigh] = calculatePrimeCount<ll>(N);
        auto [primeSumLow, primeSumHigh] = calculatePrimeSum<__int128_t>(N);

        ll arraySize = primeCountLow.size();

        // Adjust prime sums by subtracting prime counts (sum of p - sum of 1 for each prime)
        for (int i = 0; i < arraySize; i++)
            primeSumLow[i] -= primeCountLow[i];
        for (int i = 0; i < arraySize; i++)
            primeSumHigh[i] -= primeCountHigh[i];

        // Define the multiplicative function: f(p^e) = (p-1) * p^(e-1) = phi(p^e)
        auto eulerPhiFunction = [&](ll prime, ll exponent) -> __int128_t
        {
            ll result = prime - 1;
            for (int i = 0; i < exponent - 1; i++)
                result *= prime;
            return result;
        };

        // Calculate the final answer
        __int128_t phiSum = calculateMultiplicativeSum(N, eulerPhiFunction, primeSumLow, primeSumHigh);
        cout << (long long)(phiSum % mod) << endl;
    }
    return 0;
}