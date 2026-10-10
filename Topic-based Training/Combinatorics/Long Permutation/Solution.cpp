#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

using u64 = uint64_t;
// 1-based Fenwick tree tracking available unused elements
struct FenwickTree
{
private:
	int sz;
	vector<int> BIT;

public:
	// ones = true: all-ones tree in O(n)
	FenwickTree(int size, bool ones = false) : sz(size), BIT(size + 1, 0)
	{
		if (ones)
			for (int i = 1; i <= sz; i++)
				BIT[i] = i & -i;
	}

	inline void update(int i, int delta)
	{
		for (; i <= sz; i += i & -i)
			BIT[i] += delta;
	}

	inline int query(int i)
	{
		int sum = 0;
		for (; i > 0; i -= i & -i)
			sum += BIT[i];
		return sum;
	}

	// Binary lifting to find k-th (1-based) available element in O(log N)
	inline int findKth(int k)
	{
		int idx = 0;
		for (int i = std::__lg(sz); i >= 0; --i)
		{
			int nextIdx = idx + (1 << i);
			if (nextIdx <= sz && BIT[nextIdx] < k)
			{
				idx = nextIdx;
				k -= BIT[idx];
			}
		}
		return idx + 1;
	}
};

vector<u64> fact = [](int n)
{
	vector<u64> f(n + 1, 1);
	for (int i = 1; i <= n; i++)
		f[i] = f[i - 1] * i;
	return f;
}(20);

// k-th (0-based) permutation of a small set; needs n = a.size() <= 21 and k < n!
template <typename T>
vector<T> kthPermutation(vector<T> a, u64 k)
{
	sort(a.begin(), a.end());
	int n = a.size();
	FenwickTree fen(n, true);
	vector<T> p(n);
	for (int i = 0; i < n; i++)
	{
		u64 d = fact[n - i - 1];
		int j = fen.findKth(k / d + 1);
		k %= d;
		p[i] = a[j - 1];
		fen.update(j, -1);
	}
	return p;
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
		int n, q;
		cin >> n >> q;
		int m = min(n, 15), base = n - m;
		vector<ll> tail(m);
		iota(tail.begin(), tail.end(), base + 1);
		vector<ll> cur = tail;
		u64 K = 0;
		auto P = [](ll x)
		{ return x * (x + 1) / 2; };
		while (q--)
		{
			int t;
			cin >> t;
			if (t == 1)
			{
				int l, r;
				cin >> l >> r;
				ll s = 0;
				int hi = min(r, base);
				if (l <= hi)
					s += P(hi) - P(l - 1);
				for (int i = max(l, base + 1); i <= r; i++)
					s += cur[i - base - 1];
				cout << s << endl;
			}
			else
			{
				ll x;
				cin >> x;
				K += x;
				cur = kthPermutation(tail, K);
			}
		}
	}
	return 0;
}