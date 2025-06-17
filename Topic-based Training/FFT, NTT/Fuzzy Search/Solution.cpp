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
void FFT(vector<cd> &coeff)
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
		double ang = PI / k;
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
	vector<cd> in(n), out(n);

	// Convert template type to double for FFT
	for (int i = 0; i < a.size(); i++)
		in[i].real(static_cast<double>(a[i]));
	for (int i = 0; i < b.size(); i++)
		in[i].imag(static_cast<double>(b[i]));

	FFT(in);
	for (cd &x : in)
		x *= x;
	for (int i = 0; i < n; i++)
		out[i] = in[-i & (n - 1)] - conj(in[i]);
	FFT(out);

	// Convert back to template type with proper rounding
	for (int i = 0; i < res.size(); i++)
		res[i] = static_cast<T>(round(out[i].imag() / (4 * n)));

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
		int n, m, k;
		cin >> n >> m >> k;
		string S, T;
		cin >> S >> T;
		unordered_map<char, int> pos;
		int i = 0;
		for (char c : {'A', 'C', 'G', 'T'})
			pos[c] = i++;
		vector<vector<int>> PolyS(4, vector<int>(n + 1, 0));
		vector<vector<int>> PolyT(4, vector<int>(m, 0));

		for (int i = 0; i < n; i++)
		{
			int l = max(0, i - k);
			int r = min(n - 1, i + k);
			PolyS[pos[S[i]]][l] += 1;
			PolyS[pos[S[i]]][r + 1] -= 1;
		}
		for (char c : {'A', 'C', 'G', 'T'})
		{
			for (int i{1}; i < n; i++)
				PolyS[pos[c]][i] += PolyS[pos[c]][i - 1];
			for (int i{}; i < n; i++)
				PolyS[pos[c]][i] = !!PolyS[pos[c]][i];
		}
		PolyS.resize(n);
		for (int i = 0; i < m; i++)
			PolyT[pos[T[m - i - 1]]][i] = 1; // Reversed T

		vector<int> match(n + m - 1, 0);
		for (char c : {'A', 'C', 'G', 'T'})
		{
			vector<int> conv = convolute(PolyS[pos[c]], PolyT[pos[c]]);
			for (int i = 0; i < match.size(); i++)
				match[i] += conv[i];
		}
		int cnt{};
		for (int i = m - 1; i < n; i++) // i is the ending position of pattern in S
			cnt += (match[i] == m);
		cout << cnt;
	}
	return 0;
}