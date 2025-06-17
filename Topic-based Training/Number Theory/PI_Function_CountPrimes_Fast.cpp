#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct PrimeCountingStruct
{
    vector<int> primeNumbers;
    vector<int> minPrimeFactors;
    ll totalAnswer;
    ll yThreshold;
    vector<pair<pair<ll, int>, char>> PhiQueries;

    // Fenwick Tree (Binary Indexed Tree) for efficient range sum queries
    struct FenwickTree
    {
        vector<int> tree;
        int size;

        FenwickTree(int N = 0) : size(N)
        {
            tree.assign(N, 0);
        }

        // Add value k at position i
        void addValue(int position, int value)
        {
            for (; position < size; position = (position | (position + 1)))
                tree[position] += value;
        }

        // Get prefix sum from 0 to r
        int getPrefixSum(int rightBound)
        {
            int result = 0;
            for (; rightBound >= 0; rightBound = (rightBound & (rightBound + 1)) - 1)
                result += tree[rightBound];
            return result;
        }
    };

    // Main function to count primes up to N using Meissel-Lehmer algorithm
    ll countPrimesUpTo(ll N)
    {
        // Calculate optimal threshold y = N^0.64 for efficiency
        // This threshold balances memory usage and computation time
        yThreshold = pow(N, 0.64);
        if (N < 100)
            yThreshold = N;

        // Generate all primes up to yThreshold using linear sieve
        generatePrimesWithLinearSieve();

        if (N < 100)
            return primeNumbers.size();

        ll smallPrimeBound = N / yThreshold;

        // Count primes <= N/y (small primes)
        totalAnswer = 0;
        for (int prime : primeNumbers)
        {
            if (prime > smallPrimeBound)
                break;
            totalAnswer++;
        }

        int smallPrimeCount = totalAnswer; // This is PI(N/y)

        // Apply inclusion-exclusion principle for medium primes
        // Remove overcounted composites using two pointers technique
        int rightPointer = primeNumbers.size() - 1;
        for (int leftIndex = smallPrimeCount; leftIndex < primeNumbers.size(); leftIndex++)
        {
            // Find largest prime such that primes[i] * primes[j] <= N
            while (rightPointer >= leftIndex &&
                   (ll)primeNumbers[leftIndex] * primeNumbers[rightPointer] > N)
                --rightPointer;

            if (rightPointer < leftIndex)
                break;

            // Subtract overcounted pairs
            totalAnswer -= rightPointer - leftIndex + 1;
        }

        // Calculate PI(N, a) using recursive approach with memoization
        calculatePhiFunction(N, smallPrimeCount - 1);

        // Process all PI queries using Fenwick tree for efficiency
        processPhiQueries();

        PhiQueries.clear();
        return totalAnswer - 1; // Subtract 1 to exclude 1 from count
    }

private:
    // Generate primes using linear sieve of Eratosthenes
    void generatePrimesWithLinearSieve()
    {
        primeNumbers.clear();
        minPrimeFactors.assign(yThreshold + 1, -1);

        for (int currentNumber = 2; currentNumber <= yThreshold; ++currentNumber)
        {
            // If currentNumber is prime
            if (minPrimeFactors[currentNumber] == -1)
            {
                minPrimeFactors[currentNumber] = primeNumbers.size();
                primeNumbers.push_back(currentNumber);
            }

            // Mark multiples of all primes
            for (int primeIndex = 0; primeIndex < primeNumbers.size(); ++primeIndex)
            {
                int prime = primeNumbers[primeIndex];
                ll multiple = (ll)currentNumber * prime;

                if (multiple > yThreshold)
                    break;

                minPrimeFactors[multiple] = primeIndex;

                // Optimization: if currentNumber is divisible by prime, all larger primes will create duplicates
                if (currentNumber % prime == 0)
                    break;
            }
        }
    }

    // Recursive function to calculate PI(N, a) = count of numbers <= N not divisible by first (a + 1) primes
    void calculatePhiFunction(ll N, int primeIndex, int sign = 1)
    {
        if (N == 0)
            return;

        // Base case: No primes to consider
        if (primeIndex == -1)
        {
            totalAnswer += N * sign;
            return;
        }

        // If N is small enough, store query for batch processing
        if (N <= yThreshold)
        {
            PhiQueries.emplace_back(make_pair(N, primeIndex), sign);
            return;
        }

        // Inclusion-exclusion: Phi(N,a) = Phi(N, a - 1) - Phi(N / p_a, a - 1)
        calculatePhiFunction(N, primeIndex - 1, sign);
        calculatePhiFunction(N / primeNumbers[primeIndex], primeIndex - 1, -sign);
    }

    // Process all Phi queries efficiently using Fenwick tree
    void processPhiQueries()
    {
        sort(PhiQueries.begin(), PhiQueries.end());

        int currentIndex = 2;
        int totalPrimes = primeNumbers.size();

        // Fenwick tree with reversed order for efficient prefix queries
        FenwickTree fenwickTree(totalPrimes);

        for (const auto &queryInfo : PhiQueries)
        {
            auto [n_and_a, sign] = queryInfo;
            auto [N, primeIndex] = n_and_a;

            // Add all numbers up to N to fenwick tree
            while (currentIndex <= N)
            {
                fenwickTree.addValue(totalPrimes - 1 - minPrimeFactors[currentIndex], 1);
                currentIndex++;
            }

            // Query count of numbers not divisible by first (primeIndex + 1) primes
            int countNotDivisible = fenwickTree.getPrefixSum(totalPrimes - primeIndex - 2) + 1;
            totalAnswer += countNotDivisible * sign;
        }
    }
};

// Global instance for prime counting
PrimeCountingStruct *primeCountingInstance = new PrimeCountingStruct();

// Wrapper function to count primes up to N
ll countPrimesUpTo(ll N)
{
    return primeCountingInstance->countPrimesUpTo(N);
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
    }
    return 0;
}
