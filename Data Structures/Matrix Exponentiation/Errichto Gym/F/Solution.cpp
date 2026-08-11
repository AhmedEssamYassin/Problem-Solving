#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

template <int Rows, int Cols>
class Matrix
{
private:
	ll data[Rows][Cols];

public:
	// Constructor: zero matrix
	Matrix() { memset(data, 0, sizeof(data)); }

	Matrix(ll initValue)
	{
		for (int i = 0; i < Rows; i++)
			for (int j = 0; j < Cols; j++)
				data[i][j] = initValue;
	}

	// Constructor: from 2D vector
	Matrix(const vector<vector<ll>> &matrix)
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
	ll (&operator[](int row)) [Cols]
	{
		assert(row >= 0 && row < Rows);
		return data[row];
	}

	const ll (&operator[](int row) const)[Cols]
	{
		assert(row >= 0 && row < Rows);
		return data[row];
	}

	// Matrix multiplication
	template <int Q>
	Matrix<Rows, Q> operator*(const Matrix<Cols, Q> &other) const
	{
		Matrix<Rows, Q> result(2e18);
		for (int i = 0; i < Rows; i++)
		{
			for (int k = 0; k < Cols; k++)
			{
				for (int j = 0; j < Q; j++)
					result[i][j] = min(result[i][j], data[i][k] + other[k][j]);
			}
		}
		return result;
	}

	// Static method: create identity matrix
	template <int Size>
	static Matrix<Size, Size> createIdentity()
	{
		Matrix<Size, Size> identity(2e18);
		for (int i = 0; i < Size; i++)
			identity[i][i] = 0;
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
constexpr int sz = 101;
using TransMatrix = Matrix<sz, sz>;

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
		ll n, m, k;
		cin >> n >> m >> k;
		TransMatrix mat(2e18);
		for (int i{}; i < m; i++)
		{
			int u, v, w;
			cin >> u >> v >> w;
			mat[u][v] = min<ll>(mat[u][v], w);
		}
		TransMatrix res = mat.matrixPower(k);
		ll minSum = 2e18;
		for (int u = 1; u <= n; u++)
		{
			for (int v = 1; v <= n; v++)
				minSum = min(minSum, res[u][v]);
		}
		if (minSum > 1e18)
			cout << "IMPOSSIBLE";
		else
			cout << minSum << endl;
	}
	return 0;
}