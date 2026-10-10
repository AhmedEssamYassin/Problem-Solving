#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

using u32 = uint32_t;
using u64 = uint64_t;
constexpr u32 mod = 1e9 + 7;
u32 power(u32 x, u32 p)
{
	u64 r = 1;
	for (; p; p >>= 1, x = (u64)x * x % mod)
		if (p & 1)
			r = r * x % mod;
	return r;
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
	// cin >> t;
	while (t--)
	{
		int n, m, w, row = 0;
		cin >> n >> m;
		w = m + 1;
		vector<u32> a(n * w);
		for (auto &x : a)
		{
			ll v;
			cin >> v;
			x = (v % mod + mod) % mod;
		}
		vector<int> where(m, -1);
		for (int col = 0; col < m && row < n; col++)
		{
			int p = row;
			while (p < n && !a[p * w + col])
				p++;
			if (p == n)
				continue;
			u32 *R = &a[row * w];
			swap_ranges(R + col, R + w, &a[p * w + col]);
			where[col] = row;
			u64 inv = power(R[col], mod - 2);
			for (int j = col; j < w; j++)
				R[j] = R[j] * inv % mod;
			for (int i = row + 1; i < n; i++) // forward only, back substitution below
			{
				u32 *Q = &a[i * w], f = mod - Q[col];
				if (f == mod)
					continue;
				for (int j = col; j < w; j++)
					Q[j] = (Q[j] + (u64)f * R[j]) % mod;
			}
			row++;
		}
		for (int i = row; i < n; i++)
			if (a[i * w + m])
				return cout << -1 << endl, 0;
		vector<u32> x(m, 0);
		for (int c = m - 1; c >= 0; c--)
		{
			if (where[c] < 0)
				continue;
			const u32 *R = &a[where[c] * w];
			u64 s = 0;
			for (int j = c + 1; j < m; j++)
			{
				s += (u64)R[j] * x[j];
				if (s >= 8ULL * mod * mod)
					s -= 8ULL * mod * mod;
			}
			x[c] = (R[m] + mod - s % mod) % mod;
		}
		for (int j = 0; j < m; j++)
			cout << x[j] << " ";
		cout << endl;
	}
	return 0;
}