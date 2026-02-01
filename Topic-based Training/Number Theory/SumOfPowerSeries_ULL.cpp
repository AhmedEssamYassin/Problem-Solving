#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define ULL unsigned long long int
#define endl "\n"

ULL Pow(ULL N, ULL power)
{
    ULL res{1};
    while (power)
    {
        if (power & 1)
            res *= N;
        N *= N;
        power >>= 1;
    }
    return res;
}

ULL factor2s(ULL &x)
{
    if (x == 0)
        return 0ULL;
    ULL ctz{};
    while (!(x & 1))
        x >>= 1, ctz++;
    return ctz;
}

ULL calc(const vector<ULL> &y, ULL N)
{
    // Σ y[i] * ((Π (N - j)) / (Π (i - j)))
    // L{i}(N) = L{i - 1}(N) * ((N - (i - 1)) / (N - i)) * ((i - k) / i)

    int k = y.size() - 1;
    ULL ans = 0;

    // Precompute total product of all (N - j) terms
    ULL PI_N_minus_j = 1;
    ULL totalCnt2s = 0;
    for (int j = 1; j <= k; j++)
    {
        ULL num = N - j;
        totalCnt2s += factor2s(num);
        PI_N_minus_j *= num;
    }

    // Initialize running factorial values
    ULL leftFact = 1, leftCnt2s = 0;   // For (i - 1)!
    ULL rightFact = 1, rightCnt2s = 0; // For (k - i - 1)!

    // Precompute rightFact and rightCnt2s for i = 1 case (which is (k - 1)!)
    for (int i = 1; i <= k - 1; i++)
    {
        ULL curr = i;
        rightCnt2s += factor2s(curr);
        rightFact *= curr;
    }

    // Calculate answer for each i
    for (int i = 1; i <= k; i++)
    {
        // Sign calculation: (-1)^(number of negative terms)
        ULL isNeg = ((k - i) & 1) ? -1 : 1;

        // Denominator is (i - 1)! * (k - i)!
        ULL denominator = leftFact * rightFact;
        ULL denomCnt2s = leftCnt2s + rightCnt2s;

        // Numerator is total product divided by (N - i)
        ULL numerator = PI_N_minus_j;
        ULL numerCnt2s = totalCnt2s;

        // Divide by (N - i)
        ULL divisor = N - i;
        numerCnt2s -= factor2s(divisor);
        numerator *= Pow(divisor, -1);

        // Cancel 2s between numerator and denominator using bit shifting
        ULL diff = numerCnt2s - denomCnt2s;
        assert(diff >= 0);
        numerator <<= diff; // Multiply by 2^diff

        ans += isNeg * y[i] * numerator * Pow(denominator, -1);

        // For next iteration,
        // we need ((i + 1) - 1)! = (i)!
        // we need (k - (i + 1))! = (k - i - 1)!, Current rightFact is (k - i)!, so divide by (k - i)

        ULL curr = i; // 0! = 1
        leftCnt2s += factor2s(curr);
        leftFact *= curr;

        ULL rightDivisor = k - i;
        rightCnt2s -= factor2s(rightDivisor);
        rightFact *= Pow(rightDivisor, -1);
    }

    return ans;
}

ULL F(ULL N, ULL K)
{
    // How the problem define 0^0
    if (N == 0 && K == 0)
        return 1; // 1 if 0^0 is 1
    if (K == 0)
        return N + 1; // (N + 1) if 0^0 is 1
    vector<ULL> y;
    ULL sum = 0;
    y.push_back(sum);
    for (int i{1}; i <= K + 2; i++)
    {
        sum += Pow(i, K);
        y.push_back(sum);
    }
    if (N < y.size())
        return y[N];
    return calc(y, N);
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifdef LOCAL
    freopen("input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif
    int t = 1;
    ULL N, K;
    // cin >> t;
    while (t--)
    {
        cin >> N >> K;
        ULL sum{};
        for (int i{}; i <= K; i++)
            sum += F(N, i);
        cout << sum;
    }
    return 0;
}