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
        cin >> n;
        vector<ll> Poly1(n);
        for (int i{}; i < n; i++)
            cin >> Poly1[i];

        cin >> m;
        vector<ll> Poly2(m);
        for (int i{}; i < m; i++)
            cin >> Poly2[i];

        vector<ll> res = convolute(Poly1, Poly2);
        for (int i{}; i < res.size(); i++)
            cout << res[i] << " ";
    }
    return 0;
}
