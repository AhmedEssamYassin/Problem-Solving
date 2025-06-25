#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

const int BITS = 61;
#define int_type std::conditional_t<(BITS > 31), long long, int>
struct XORBasis
{
	int sz = 0;
	array<int_type, BITS> basis{};
	XORBasis() {}
	XORBasis(const ll &x)
	{
		insertVector(x);
	}
	void insertVector(ll x)
	{
		if (sz == BITS)
			return;
		for (ll i = __lg(x); x > 0; i = __lg(x))
		{
			if (!basis[i])
				return sz++, void(basis[i] = x);
			x ^= basis[i];
		}
	}

	bool canRepresent(ll x)
	{
		if (sz == BITS)
			return true;
		for (ll i = __lg(x); x > 0; i = __lg(x))
		{
			if (basis[i])
				x ^= basis[i];
			else
				return false;
		}
		return !x;
	}
	vector<ll> getReducedBasis()
	{
		for (int i = BITS - 1; i >= 0; i--)
		{
			if (!basis[i])
				continue;
			for (int j = i - 1; j >= 0; j--)
			{
				if (basis[j] && ((basis[i] >> j) & 1))
					basis[i] ^= basis[j];
			}
		}
		vector<ll> res;
		for (int i = 0; i < BITS; i++)
		{
			if (basis[i])
				res.push_back(basis[i]);
		}
		return res;
	}

	ll kthSmallest(ll k)
	{
		auto vec = getReducedBasis();
		int n = vec.size();
		if (k >= (1LL << n))
			return -1;
		ll res = 0;
		for (int i = 0; i < n; i++)
		{
			if ((k >> i) & 1)
				res ^= vec[i];
		}
		return res;
	}
	ll getMaxXor()
	{
		if (sz == BITS)
			return (1LL << BITS) - 1;
		ll maxXor = 0;
		for (int i = BITS - 1; i >= 0; i--)
		{
			if ((maxXor ^ basis[i]) > maxXor)
				maxXor ^= basis[i];
		}
		return maxXor;
	}

	friend XORBasis operator+(const XORBasis &LHS, const XORBasis &RHS)
	{
		XORBasis res;
		if (LHS.sz == BITS)
			return LHS;

		if (RHS.sz == BITS)
			return (RHS);
		res = LHS;
		for (int i = 0; i < BITS; i++)
		{
			if (RHS.basis[i])
				res.insertVector(RHS.basis[i]);
		}
		return res;
	}

	XORBasis &operator+=(const XORBasis &other)
	{
		if (sz == BITS)
			return *this;

		if (other.sz == BITS)
			return (*this = other);

		for (int i = 0; i < BITS; i++)
		{
			if (other.basis[i])
				insertVector(other.basis[i]);
		}
		return *this;
	}

	void clear()
	{
		if (!sz)
			return;
		basis.fill(0);
		sz = 0;
	}
};

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
		XORBasis xb;
		for (int i{}; i < N; i++)
		{
			ll type, x, k;
			cin >> type;
			if (type == 1)
			{
				cin >> x;
				xb.insertVector(x);
			}
			else
			{
				cin >> k;
				cout << xb.kthSmallest(--k) << endl;
			}
		}
	}
	return 0;
}