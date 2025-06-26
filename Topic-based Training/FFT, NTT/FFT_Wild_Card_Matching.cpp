#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

using cd = std::complex<double>;
const double PI = acos(-1);

/**
 * Fast Fourier Transform - Optimized Implementation
 * Transforms between coefficient and point-value representation
 * Time: O(n log n), Space: O(n)
 */
void FFT(vector<cd> &coeff, bool inverse = false)
{
    int n = coeff.size();

    // Bit-reversal permutation - eliminates recursion overhead
    for (int i = 1, j = 0; i < n; i++)
    {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;
        if (i < j)
            swap(coeff[i], coeff[j]);
    }

    // Iterative FFT with on-the-fly root computation
    for (int k = 1; k < n; k *= 2)
    {
        double ang = PI / k * (inverse ? -1 : 1);
        cd wlen(cos(ang), sin(ang));
        for (int i = 0; i < n; i += 2 * k)
        {
            cd w(1);
            for (int j = 0; j < k; j++)
            {
                cd z = coeff[i + j + k] * w;
                coeff[i + j + k] = coeff[i + j] - z;
                coeff[i + j] += z;
                w *= wlen;
            }
        }
    }
    if (inverse)
    {
        for (cd &x : coeff)
            x /= n;
    }
}

/**
 * Convolution / Multiplication using optimized FFT
 * Multiplies two vectors represented as double vectors
 * Time: O(n log n), where n is the size of result
 */
template <typename T>
vector<T> convolute(const vector<T> &a, const vector<T> &b)
{
    if (a.empty() || b.empty())
        return {};

    vector<T> res(a.size() + b.size() - 1);
    int L = 32 - __builtin_clz(res.size()), n = 1 << L;
    vector<cd> in(n);

    // Convert template type to double for FFT
    for (int i = 0; i < a.size(); i++)
        in[i].real(static_cast<double>(a[i]));
    for (int i = 0; i < b.size(); i++)
        in[i].imag(static_cast<double>(b[i]));

    FFT(in);
    for (cd &x : in)
        x *= x;
    FFT(in, true);

    // Convert back to template type with proper rounding
    for (int i = 0; i < res.size(); i++)
        res[i] = static_cast<T>(round(in[i].imag() / 2.0));

    return res;
}

template <typename T>
vector<T> PolyModPow(vector<T> P, ll power)
{
    vector<T> res{1};
    while (power)
    {
        if (power & 1)
            res = convolute(res, P);
        P = convolute(P, P);
        power >>= 1;
    }
    return res;
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
        int n, m;
        string S, T;
        cin >> S >> T;
        n = S.length();
        m = T.length();
        vector<ll> S1(n);
        for (int i = 0; i < n; i++)
        {
            if (S[i] == '*')
                S1[i] = 0;
            else
                S1[i] = (S[i] - 'a' + 1);
        }

        vector<ll> T1(m);
        for (int i = 0; i < m; i++)
        {
            if (T[i] == '*')
                T1[i] = 0;
            else
                T1[i] = (T[i] - 'a' + 1);
        }
        reverse(T1.begin(), T1.end());

        vector<ll> S2 = S1;
        vector<ll> T2 = T1;

        vector<ll> S3 = S1;
        vector<ll> T3 = T1;

        auto square = [](vector<ll> &v)
        {
            for (int i = 0; i < v.size(); i++)
                v[i] = v[i] * v[i];
        };

        auto cube = [](vector<ll> &v)
        {
            for (int i = 0; i < v.size(); i++)
                v[i] = v[i] * v[i] * v[i];
        };

        square(S2);
        square(T2);
        cube(S3);
        cube(T3);

        auto S3T1 = convolute(S3, T1);
        auto S2T2 = convolute(S2, T2);
        auto S1T3 = convolute(S1, T3);

        string ans("");
        for (int i = 0; i < n - m + 1; i++)
        {
            int a = S3T1[i + m - 1];
            int b = S2T2[i + m - 1];
            int c = S1T3[i + m - 1];
            if (a - 2 * b + c == 0)
                ans += "1";
            else
                ans += "0";
        }
        cout << ans << endl;
    }
    return 0;
}