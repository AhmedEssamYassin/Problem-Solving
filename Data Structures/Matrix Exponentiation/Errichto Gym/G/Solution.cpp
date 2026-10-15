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
constexpr int mod = 1e9 + 7;
using Mint = Modular<integral_constant<decay_t<decltype(mod)>, mod>>;
Mint operator""_m(unsigned long long x) { return Mint(x); }

template <int Rows, int Cols>
class Matrix
{
private:
	Mint data[Rows][Cols];

public:
	// Constructor: zero matrix
	Matrix() { memset(data, 0, sizeof(data)); }

	Matrix(Mint initValue)
	{
		for (int i = 0; i < Rows; i++)
			for (int j = 0; j < Cols; j++)
				data[i][j] = initValue;
	}

	// Constructor: from 2D vector
	Matrix(const vector<vector<Mint>> &matrix)
	{
		assert(matrix.size() == Rows && (matrix.empty() || matrix[0].size() == Cols));
		for (int i = 0; i < Rows; i++)
			for (int j = 0; j < Cols; j++)
				data[i][j] = matrix[i][j];
	}

	// Copy constructor
	Matrix(const Matrix &other) = default;

	// Assignment operator
	Matrix &operator=(const Matrix &other) = default;

	// Element access using operator[]
	Mint (&operator[](int row))[Cols]
	{
		assert(row >= 0 && row < Rows);
		return data[row];
	}

	const Mint (&operator[](int row) const)[Cols]
	{
		assert(row >= 0 && row < Rows);
		return data[row];
	}

	// Matrix multiplication
	template <int Q>
	Matrix<Rows, Q> operator*(const Matrix<Cols, Q> &other) const
	{
		Matrix<Rows, Q> result;
		constexpr int b = min<u64>(16, ~0ULL / (1ULL * (mod - 1) * (mod - 1))); // For constant mod only
		// constexpr int b = 16; // If mod is larger that 1.07e9, b must take smaller values
		for (int i = 0; i < Rows; i++)
		{
			for (int bk = 0; bk < Cols; bk += b)
			{
				int kEnd = min(Cols, bk + b);
				alignas(64) uint64_t localC[Q] = {0};
				for (int k = bk; k < kEnd; k++)
				{
					uint32_t valA = data[i][k]();
					if (!valA)
						continue;
					for (int j = 0; j < Q; j++)
						localC[j] += 1ULL * valA * other[k][j]();
				}
				for (int j = 0; j < Q; j++)
					result[i][j] += localC[j] % mod;
			}
		}
		return result;
	}

	// Static method: create identity matrix
	template <int Size>
	static Matrix<Size, Size> createIdentity()
	{
		Matrix<Size, Size> identity;
		for (int i = 0; i < Size; i++)
			identity[i][i] = 1;
		return identity;
	}

	// Matrix exponentiation (only for square matrices)
	Matrix<Rows, Rows> matrixPower(ll exp) const
	{
		static_assert(Rows == Cols, "Matrix must be square for exponentiation");
		assert(exp >= 0);

		Matrix<Rows, Rows> result = createIdentity<Rows>();
		Matrix<Rows, Rows> base = *this;

		while (exp > 0)
		{
			if (exp & 1)
				result = result * base;
			base = base * base;
			exp >>= 1;
		}
		return result;
	}

	// Input stream operator
	friend istream &operator>>(istream &is, Matrix &matrix)
	{
		for (int i{}; i < Rows; i++)
			for (int j{}; j < Cols; j++)
				is >> matrix[i][j];
		return is;
	}
};
constexpr int sz = 14;
using Matrix14by14 = Matrix<sz, sz>;
using Matrix14by1 = Matrix<sz, 1>;

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
		ll n, k;
		cin >> n >> k;
		vector<ll> a(n), c(n);
		for (int i{}; i < n; i++)
			cin >> a[i];
		for (int i{}; i < n; i++)
			cin >> c[i];
		ll p, q, r;
		cin >> p >> q >> r;

		/*
		    [    a[i]     ]     [ c1  c2  c3  ...  cn   r   q   p ] [ a[i - 1]  ]
		    [   a[i - 1]  ]     [  1   0   0  ...   0   0   0   0 ] [ a[i - 2]  ]
		    [   a[i - 2]  ]     [  0   1   0  ...   0   0   0   0 ] [ a[i - 3]  ]
		    [      ...    ]  =  [  0   0   1  ...   0   0   0   0 ] [    ...    ]
		    [ a[i - n + 1]]     [  0   0   0  ...   1   0   0   0 ] [ a[i - n]  ]
		    [   (i + 1)²  ]     [  0   0   0  ...   0   1   2   1 ] [    i²     ]
		    [    i + 1    ]     [  0   0   0  ...   0   0   1   1 ] [    i      ]
		    [      1      ]     [  0   0   0  ...   0   0   0   1 ] [    1      ]
		*/
		// Handle base cases
		if (k < n)
		{
			cout << a[k] << endl;
			continue;
		}

		Matrix14by14 mat;
		for (int j = 0; j < n; j++)
			mat[0][j] = c[j];
		mat[0][n] = r;     // coefficient for i²
		mat[0][n + 1] = q; // coefficient for i
		mat[0][n + 2] = p; // coefficient for constant 1

		// Rows 1 to (n - 1): shift the sequence values
		for (int i = 1; i < n; i++)
			mat[i][i - 1] = 1;

		// Row n: transition for i² -> (i + 1)² = i² + 2i + 1
		mat[n][n] = 1;     // i²
		mat[n][n + 1] = 2; // 2 * i
		mat[n][n + 2] = 1; // +1 (constant)

		// Row n+1: transition for i -> i + 1 = i + 1
		mat[n + 1][n + 1] = 1; // i
		mat[n + 1][n + 2] = 1; // +1 (constant)

		// Row n + 2: constant 1 remains 1
		mat[n + 2][n + 2] = 1;

		// Initial state vector for position (n - 1) (0-indexed)
		Matrix14by1 state;
		for (int i = 0; i < n; i++)
			state[i][0] = a[n - i - 1]; // a[n - 1], a[n - 2], ..., a[0]

		ll idx = n;                    // current index (0-based)
		state[n][0] = Mint(idx * idx); // (n - 1)²
		state[n + 1][0] = Mint(idx);   // (n - 1)
		state[n + 2][0] = Mint(1);     // constant 1

		// Apply transition (k - n + 1) times to get to position k
		auto result = mat.matrixPower(k - n + 1) * state;

		cout << result[0][0] << endl;
	}
	return 0;
}