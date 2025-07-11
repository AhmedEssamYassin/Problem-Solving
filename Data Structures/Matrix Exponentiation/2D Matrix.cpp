#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

const ll mod = 1e9 + 7;
#define double_size_t std::conditional_t<(mod > (1LL << 31)), __int128_t, long long>
inline ll add64(const ll &a, const ll &b)
{
    double_size_t res = double_size_t(a) + b;
    if (res >= mod)
        res -= mod;
    return res;
}

inline ll sub64(const ll &a, const ll &b)
{
    double_size_t res = double_size_t(a) - b;
    if (res < 0)
        res += mod;
    if (res >= mod)
        res -= mod;
    return res;
}

inline ll mult64(const ll &a, const ll &b)
{
    return double_size_t(a) * b % mod;
}

struct Matrix2D
{
    ll a[2][2] = {{0, 0}, {0, 0}};
    // ==================== CONSTRUCTORS ====================
    // Empty constructor
    Matrix2D() {}

    // Value constructor
    Matrix2D(ll a00, ll a01, ll a10, ll a11)
    {
        a[0][0] = a00;
        a[0][1] = a01;
        a[1][0] = a10;
        a[1][1] = a11;
    }

    // Deep Copy constructor
    Matrix2D(const Matrix2D &other)
    {
        (*this)[0][0] = other[0][0];
        (*this)[0][1] = other[0][1];
        (*this)[1][0] = other[1][0];
        (*this)[1][1] = other[1][1];
    }

    // ==================== ASSIGNMENT OPERATOR ====================
    Matrix2D &operator=(const Matrix2D &other)
    {
        (*this)[0][0] = other[0][0];
        (*this)[0][1] = other[0][1];
        (*this)[1][0] = other[1][0];
        (*this)[1][1] = other[1][1];
        return *this;
    }

    // ==================== ELEMENT ACCESS ====================
    // Element access using operator[]
    ll *operator[](int row)
    {
        assert(row >= 0 && row < 2);
        return a[row];
    }

    const ll *operator[](int row) const
    {
        assert(row >= 0 && row < 2);
        return a[row];
    }

    // ==================== ARITHMETIC OPERATORS ====================
    // Matrix2D multiplication
    Matrix2D operator*(const Matrix2D &other) const
    {
        Matrix2D product(0, 0, 0, 0);
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 2; k++)
                    product.a[i][k] = add64(product.a[i][k], mult64(a[i][j], other.a[j][k]));
            }
        }
        return product;
    }

    Matrix2D &operator*=(const Matrix2D &other)
    {
        *this = *this * other;
        return *this;
    }

    // Input stream operator
    friend istream &operator>>(istream &is, Matrix2D &matrix)
    {
        for (int i{}; i < 2; i++)
            for (int j{}; j < 2; j++)
                is >> matrix[i][j];
        return is;
    }
};

Matrix2D matrixPow(Matrix2D mat, ll power)
{
    Matrix2D res(1, 0, 0, 1); // Identity Matrix2D
    while (power)
    {
        if (power & 1)
            res *= mat;
        mat *= mat;
        power >>= 1;
    }
    return res;
}

ll F(ll N)
{
    Matrix2D fibMatrix(1, 1, 1, 0);
    Matrix2D res = matrixPow(fibMatrix, N);
    return res[0][1];
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
    ll N;
    // cin >> t;
    while (t--)
    {
        cin >> N;
        cout << F(N);
    }
    return 0;
}