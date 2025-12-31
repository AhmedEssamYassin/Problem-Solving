#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

template <int Rows, int Cols>
class Matrix
{
private:
	uint32_t data[Rows][Cols];

public:
	// Constructor: zero matrix
	Matrix() { memset(data, 0, sizeof(data)); }

	// Constructor: with initial value (m parameter comes first to avoid ambiguity)
	Matrix(ll m, uint32_t initValue)
	{
		for (int i = 0; i < Rows; i++)
			for (int j = 0; j < Cols; j++)
				data[i][j] = initValue;
	}

	// Constructor: from 2D vector
	Matrix(const vector<vector<uint32_t>> &matrix)
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
	uint32_t (&operator[](int row)) [Cols]
	{
		assert(row >= 0 && row < Rows);
		return data[row];
	}

	const uint32_t (&operator[](int row) const)[Cols]
	{
		assert(row >= 0 && row < Rows);
		return data[row];
	}

	// Matrix multiplication
	template <int Q>
	Matrix<Rows, Q> operator*(const Matrix<Cols, Q> &other) const
	{
		Matrix<Rows, Q> result;
		for (int i = 0; i < Rows; i++)
		{
			for (int j = 0; j < Q; j++)
			{
				for (int k = 0; k < Cols; k++)
					result[i][j] += data[i][k] * other[k][j];
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
			identity.data[i][i] = 1;
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
constexpr int sz = 65;
using TransMatrix = Matrix<sz, sz>;

// Convert 2D coordinates to 1D index
int coordToIndex(int r, int c)
{
	return r * 8 + c;
}

// Check if coordinates are valid
bool isValid(int r, int c)
{
	return r >= 0 && r < 8 && c >= 0 && c < 8;
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
	// cin >> t;
	while (t--)
	{
		ll k;
		cin >> k;
		TransMatrix mat;
		// Knight moves: 8 possible moves
		int dr[] = {-2, -2, -1, -1, 1, 1, 2, 2};
		int dc[] = {-1, 1, -2, 2, -2, 2, -1, 1};
		for (int r = 0; r < 8; r++)
		{
			for (int c = 0; c < 8; c++)
			{
				int from = coordToIndex(r, c);

				for (int move = 0; move < 8; move++)
				{
					int nr = r + dr[move];
					int nc = c + dc[move];

					if (isValid(nr, nc))
					{
						int to = coordToIndex(nr, nc);
						mat[from][to] = 1; // Can move from (r, c) to (nr, nc)
					}
				}
			}
		}
		for (int i = 0; i < 65; i++)
			mat[i][64] = 1;
		TransMatrix res = mat.matrixPower(k);
		uint32_t cnt = 0;
		for (int i = 0; i < 65; i++)
			cnt += res[0][i];

		cout << cnt << endl;
	}
	return 0;
}