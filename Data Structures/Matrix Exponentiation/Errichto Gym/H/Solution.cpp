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
constexpr int sz = 2;
using Matrix2by2 = Matrix<sz, sz>;

struct SegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		Matrix2by2 mat;
		// Constructors
		Node() { mat = mat.createIdentity<sz>(); }
		Node(const Matrix2by2 &o) : mat(o) {}
	};
	int size;
	vector<Node> seg;
	Node merge(const Node &leftNode, const Node &rightNode)
	{
		Node res;
		res.mat = (leftNode.mat * rightNode.mat);
		return res;
	}
	void build(int left, int right, int node, const vector<Matrix2by2> &arr)
	{
		if (left == right) // Leaf Node (single element)
		{
			if (left < arr.size()) // Making sure we are inside the boundaries of the array
				seg[node] = arr[left];
			return;
		}
		// Building left node
		build(left, mid, L, arr);

		// Building right node
		build(mid + 1, right, R, arr);

		// Returning to parent nodes
		seg[node] = merge(seg[L], seg[R]);
	}
	void update(int left, int right, int node, int idx, const Matrix2by2 &x)
	{
		if (left == right)
		{
			seg[node] = x;
			return;
		}
		if (idx <= mid)
			update(left, mid, L, idx, x);
		else
			update(mid + 1, right, R, idx, x);
		// Updating while returning to parent nodes
		seg[node] = merge(seg[L], seg[R]);
	}
	Node query(int left, int right, int node, int leftQuery, int rightQuery)
	{
		// Out of range
		if (right < leftQuery || left > rightQuery)
			return Node(); // A value that doesn't affect the query

		// The whole range is the answer
		if (left >= leftQuery && right <= rightQuery)
			return seg[node];

		// ONLY a part of this segment belongs to the query
		Node leftSegment = query(left, mid, L, leftQuery, rightQuery);
		Node rightSegment = query(mid + 1, right, R, leftQuery, rightQuery);
		return merge(leftSegment, rightSegment);
	}

public:
	SegmentTree(const vector<Matrix2by2> &arr)
	{
		size = 1;
		int n = arr.size();
		while (size < n)
			size <<= 1;
		seg = vector<Node>(2 * size);
		build(0, size - 1, 0, arr);
	}
	void update(int idx, const Matrix2by2 &val)
	{
		update(0, size - 1, 0, idx, val);
	}
	Mint query(int left, int right)
	{
		Node ans = query(0, size - 1, 0, left, right);
		return ans.mat[0][0];
	}

#undef L
#undef R
#undef mid
};

Matrix2by2 getTransition(char c)
{
	Matrix2by2 trans;

	if (c == '?')
	{
		trans[0][0] = 19; // happy -> happy
		trans[0][1] = 7;  // happy -> sad
		trans[1][0] = 6;  // sad -> happy
		trans[1][1] = 20; // sad -> sad
	}
	else
	{
		// Specific character
		if (c == 'S' || c == 'D')
		{
			// Makes sad
			trans[0][1] = 1; // happy -> sad
			trans[1][1] = 1; // sad -> sad
		}
		else if (c == 'H')
		{
			// Makes happy
			trans[0][0] = 1; // happy -> happy
			trans[1][0] = 1; // sad -> happy
		}
		else if (c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U')
		{
			// Flips mood
			trans[0][1] = 1; // happy -> sad
			trans[1][0] = 1; // sad -> happy
		}
		else
		{
			// No change
			trans[0][0] = 1; // happy -> happy
			trans[1][1] = 1; // sad -> sad
		}
	}

	return trans;
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
		string str;
		cin >> str;
		vector<Matrix2by2> vc;
		for (int i{}; i < n; i++)
			vc.push_back(getTransition(str[i]));
		SegmentTree segTree(vc);
		cout << segTree.query(0, n - 1) << endl;
		while (q--)
		{
			int i;
			char c;
			cin >> i >> c;
			i--;
			segTree.update(i, getTransition(c));
			cout << segTree.query(0, n - 1) << endl;
		}
	}
	return 0;
}