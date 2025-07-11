#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

inline ll add64(const ll &a, const ll &b, const ll &mod)
{
    __int128_t res = __int128_t(a) + b;
    if (res >= mod)
        res -= mod;
    return res;
}

inline ll sub64(const ll &a, const ll &b, const ll &mod)
{
    __int128_t res = __int128_t(a) - b;
    if (res < 0)
        res += mod;
    if (res >= mod)
        res -= mod;
    return res;
}

inline ll mult64(const ll &a, const ll &b, const ll &mod)
{
    return __int128_t(a) * b % mod;
}

class Matrix
{
private:
    int numRows;
    int numCols;
    ll modulus;
    vector<vector<ll>> data;

public:
    // Constructor: zero matrix
    Matrix(int rows, int cols, ll mod = 1e9 + 7)
        : numRows(rows), numCols(cols), modulus(mod), data(rows, vector<ll>(cols, 0)) {}

    // Constructor: with initial value (mod parameter comes first to avoid ambiguity)
    Matrix(int rows, int cols, ll mod, ll initialValue)
        : numRows(rows), numCols(cols), modulus(mod), data(rows, vector<ll>(cols, initialValue)) {}

    // Constructor: from 2D vector
    Matrix(const vector<vector<ll>> &matrix, ll mod = 1e9 + 7)
        : numRows(matrix.size()), numCols(matrix[0].size()), modulus(mod), data(matrix) {}

    // Copy constructor
    Matrix(const Matrix &other) = default;

    // Assignment operator
    Matrix &operator=(const Matrix &other) = default;

    // Element access using operator[]
    vector<ll> &operator[](int row)
    {
        assert(row >= 0 && row < numRows);
        return data[row];
    }

    const vector<ll> &operator[](int row) const
    {
        assert(row >= 0 && row < numRows);
        return data[row];
    }

    // Matrix multiplication
    Matrix operator*(const Matrix &other) const
    {
        assert(numCols == other.numRows);
        assert(modulus == other.modulus);

        Matrix result(numRows, other.numCols, modulus);
        for (int i = 0; i < numRows; i++)
        {
            for (int j = 0; j < other.numCols; j++)
            {
                for (int k = 0; k < numCols; k++)
                    result.data[i][j] = add64(result.data[i][j], mult64(data[i][k], other.data[k][j], modulus), modulus);
            }
        }
        return result;
    }

    // Static method: create identity matrix
    static Matrix createIdentity(int size, ll mod = 1e9 + 7)
    {
        Matrix identity(size, size, mod);
        for (int i = 0; i < size; i++)
            identity.data[i][i] = 1;
        return identity;
    }

    // Matrix exponentiation (only for square matrices)
    Matrix matrixPower(ll exponent) const
    {
        assert(numRows == numCols);
        assert(exponent >= 0);

        Matrix result = createIdentity(numRows, modulus);
        Matrix base = *this;

        while (exponent > 0)
        {
            if (exponent & 1)
                result = result * base;
            base = base * base;
            exponent >>= 1;
        }
        return result;
    }

    // Sum of powers: A^0 + A^1 + A^2 + ... + A^N
    static Matrix sumOfPowers(const Matrix &matrix, ll exponent)
    {
        assert(matrix.numRows == matrix.numCols);

        int dimension = matrix.numRows;
        ll mod = matrix.modulus;
        Matrix identity = createIdentity(dimension, mod);

        // Build augmented 2d x 2d matrix M
        // M = [A  I]
        //     [0  I]
        Matrix augmented(2 * dimension, 2 * dimension, mod);

        for (int i = 0; i < dimension; i++)
        {
            for (int j = 0; j < dimension; j++)
            {
                augmented.data[i][j] = matrix.data[i][j];                        // Top-left: A
                augmented.data[i][j + dimension] = (i == j) ? 1 : 0;             // Top-right: I
                augmented.data[i + dimension][j] = 0;                            // Bottom-left: 0
                augmented.data[i + dimension][j + dimension] = (i == j) ? 1 : 0; // Bottom-right: I
            }
        }

        // Compute M^{N+1}
        Matrix matrixExponent = augmented.matrixPower(exponent + 1);

        // Extract top-right block: sum A^0 + A^1 + ... + A^N
        Matrix result(dimension, dimension, mod);
        for (int i = 0; i < dimension; i++)
        {
            for (int j = 0; j < dimension; j++)
                result.data[i][j] = matrixExponent.data[i][j + dimension];
        }

        return result;
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
        const int V = 4;
        Matrix A(V, V, 1e9 + 7);
        for (int i = 0; i < V; i++)
        {
            for (int j = 0; j < V; j++)
            {
                if (i != j)
                    A[i][j] = 1; // K4 adjacency matrix, no self-loops
            }
        }
        Matrix walkSum = Matrix::sumOfPowers(A, N);
        cout << walkSum[0][0] - 1;
    }
    return 0;
}