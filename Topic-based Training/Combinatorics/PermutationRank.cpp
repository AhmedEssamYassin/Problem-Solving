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

// 0-based lexicographical permutation rank
u64 permutationRank(const vector<int> &perm)
{
	int n = perm.size();
	FenwickTree fenTree(n, true);
	u64 rank = 0;
	for (int i = 0; i < n; i++)
	{
		rank += (u64)fenTree.query(perm[i] - 1) * fact[n - i - 1];
		fenTree.update(perm[i], -1);
	}
	return rank;
}

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
	cin >> t;
	while (t--)
	{
		ll n, k, type;
		cin >> type;
		if (type == 1)
		{
			cin >> n >> k;
			vector<u64> vc(n);
			iota(vc.begin(), vc.end(), 1);
			auto res = kthPermutation(vc, k - 1);
			for (const auto &x : res)
				cout << x << " ";
			cout << endl;
		}
		else
		{
			cin >> n;
			vector<int> vc(n);
			for (int i{}; i < n; i++)
				cin >> vc[i];
			cout << permutationRank(vc) + 1 << endl;
		}
	}
	return 0;
}