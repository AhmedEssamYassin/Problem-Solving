#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

using i64 = int64_t;
using u32 = uint32_t;
using u64 = uint64_t;
using u128 = __uint128_t;
template <typename T>
T inverse(T a, T m)
{
	T u = 0, v = 1;
	while (a != 0)
	{
		T t = m / a;
		m -= t * a, swap(a, m), u -= t * v, swap(u, v);
	}
	assert(m == 1);
	return u;
}

template <typename T>
class Modular
{
public:
	using Type = decay_t<decltype(T::value)>;
	constexpr static Type mod() { return T::value; }

	Modular() = default;
	template <typename U>
	Modular(const U &x) : value(normalize(x)) {}
	template <typename U>
	static Type normalize(const U &x)
	{
		Type v = (-mod() <= x && x < mod()) ? static_cast<Type>(x) : static_cast<Type>(x % mod());
		if (v < 0)
			v += mod();
		return v;
	}
	const Type &operator()() const { return value; }
	template <typename U>
	explicit operator U() const { return static_cast<U>(value); }

	Modular &operator+=(const Modular &o) { return value += o.value, value -= (value >= mod()) * mod(), *this; }
	Modular &operator-=(const Modular &o) { return value -= o.value, value += (value < 0) * mod(), *this; }
	Modular &operator*=(const Modular &o)
	{
		if constexpr (is_integral_v<Type> && sizeof(Type) == 4)
			value = normalize(int64_t(value) * o.value);
		else if constexpr (is_integral_v<Type> && sizeof(Type) == 8)
		{
			int64_t q = int64_t((long double)value * o.value / mod());
			value = normalize(int64_t(uint64_t(value) * o.value - uint64_t(q) * mod()));
		}
		else
			value = normalize(value * o.value);
		return *this;
	}
	Modular &operator/=(const Modular &o) { return *this *= Modular(inverse(o.value, mod())); }
	Modular &operator++() { return *this += 1; }
	Modular &operator--() { return *this -= 1; }
	Modular operator++(int)
	{
		Modular r(*this);
		return ++*this, r;
	}
	Modular operator--(int)
	{
		Modular r(*this);
		return --*this, r;
	}
	Modular operator-() const { return Modular(-value); }

#define MOD_OP(o) \
	friend Modular operator o(Modular a, const Modular &b) { return a o## = b; }
	MOD_OP(+)
	MOD_OP(-)
	MOD_OP(*)
	MOD_OP(/)
#undef MOD_OP
	friend bool operator==(const Modular &a, const Modular &b) { return a.value == b.value; }
	friend bool operator!=(const Modular &a, const Modular &b) { return !(a == b); }
	friend bool operator<(const Modular &a, const Modular &b) { return a.value < b.value; }
	friend const Type &abs(const Modular &x) { return x.value; }
	friend bool IsZero(const Modular &x) { return x() == 0; }
	friend string to_string(const Modular &x) { return std::to_string(x()); }

	template <typename U>
	friend Modular power(Modular x, U p)
	{
		assert(p >= 0);
		Modular r = 1;
		for (; p > 0; p >>= 1, x *= x)
			if (p & 1)
				r *= x;
		return r;
	}
	template <typename S>
	friend S &operator<<(S &s, const Modular &x) { return s << x(), s; }
	template <typename S>
	friend S &operator>>(S &s, Modular &x)
	{
		common_type_t<Type, int64_t> v;
		return s >> v, x.value = normalize(v), s;
	}

private:
	Type value{};
};

// Runtime mod: struct VarMod { static ModType value; }; ModType VarMod::value; ModType &mod = VarMod::value; using Mint = Modular<VarMod>;
constexpr int mod = 998244353;
using Mint = Modular<integral_constant<decay_t<decltype(mod)>, mod>>;
Mint operator""_m(unsigned long long x) { return Mint(x); }

vector<int> primes, mu;
void linearSieveOfEratosthenes(ll N)
{
	mu.assign(N + 1, 0);
	bitset<2000001> isPrime;
	isPrime.set();
	isPrime[0] = isPrime[1] = 0;
	mu[1] = 1;
	for (ll i = 2; i <= N; i++)
	{
		if (isPrime[i])
		{
			primes.push_back(i);
			mu[i] = -1;
		}
		for (ll j = 0; j < (ll)primes.size() && i * primes[j] <= N; j++)
		{
			ll multiple = i * primes[j];
			isPrime[multiple] = 0;
			if (i % primes[j] == 0)
			{
				mu[multiple] = 0;
				break;
			}
			mu[multiple] = -mu[i];
		}
	}
}

static int autoCall = (linearSieveOfEratosthenes(2e6), 0);

/**
 * Fast Mobius Transform for GCD Convolution (Prime SOS DP)
 * Time: O(N log log N), Space: O(1) in-place
 */
template <typename T>
void mobiusTransform(vector<T> &polyVec, bool inverse = false)
{
	int vecSize = polyVec.size();
	if (!inverse)
	{
		// Fast Zeta Transform (Sum over multiples)
		for (const int &p : primes)
		{
			if (p >= vecSize)
				break;
			// Iterate downwards to accumulate multiples correctly
			for (int i = (vecSize - 1) / p; i >= 1; i--)
				polyVec[i] += polyVec[i * p];
		}
	}
	else
	{
		// Fast Mobius Transform (Exact Inversion)
		for (const int &p : primes)
		{
			if (p >= vecSize)
				break;
			// Iterate upwards to subtract multiples sequentially
			for (int i = 1; i * p < vecSize; i++)
				polyVec[i] -= polyVec[i * p];
		}
	}
}

/**
 * GCD Convolution using Mobius Transform
 * Result[i] = sum(a[j] * b[k]) for all gcd(j, k) = i
 */
template <typename T>
vector<T> convoluteGCD(const vector<T> &polyA, const vector<T> &polyB)
{
	if (polyA.empty() || polyB.empty())
		return {};

	int maxSize = max(polyA.size(), polyB.size());

	vector<T> transA(polyA.begin(), polyA.end()), transB(polyB.begin(), polyB.end());
	transA.resize(maxSize);
	transB.resize(maxSize);

	mobiusTransform(transA);
	mobiusTransform(transB);

	for (int idx = 0; idx < maxSize; idx++)
		transA[idx] = (transA[idx] * transB[idx]);

	mobiusTransform(transA, true);

	return transA;
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
	ll N;
	// cin >> t;
	while (t--)
	{
		cin >> N;
		// GCD works with 1-based Polynomials (Since gcd = 0 is undefined)
		vector<Mint> Poly1(N + 1), Poly2(N + 1);
		for (int i{1}; i <= N; i++)
			cin >> Poly1[i];
		for (int i{1}; i <= N; i++)
			cin >> Poly2[i];
		vector<Mint> res = convoluteGCD(Poly1, Poly2);
		for (int i{1}; i < res.size(); i++)
			cout << res[i] << " ";
	}

	return 0;
}