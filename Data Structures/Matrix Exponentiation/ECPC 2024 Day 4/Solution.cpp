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
        m -= t * a;
        swap(a, m);
        u -= t * v;
        swap(u, v);
    }
    assert(m == 1);
    return u;
}

template <typename T>
class Modular
{
public:
    using Type = typename decay<decltype(T::value)>::type;

    constexpr Modular() : value() {}
    template <typename U>
    Modular(const U &x)
    {
        value = normalize(x);
    }

    template <typename U>
    static Type normalize(const U &x)
    {
        Type v;
        if (-mod() <= x && x < mod())
            v = static_cast<Type>(x);
        else
            v = static_cast<Type>(x % mod());
        if (v < 0)
            v += mod();
        return v;
    }

    const Type &operator()() const { return value; }
    template <typename U>
    explicit operator U() const { return static_cast<U>(value); }
    constexpr static Type mod() { return T::value; }

    Modular &operator+=(const Modular &other)
    {
        value += other.value;
        value -= (value >= mod()) * mod();
        return *this;
    }
    Modular &operator-=(const Modular &other)
    {
        value -= other.value;
        value += (value < 0) * mod();
        return *this;
    }
    template <typename U>
    Modular &operator+=(const U &other) { return *this += Modular(other); }
    template <typename U>
    Modular &operator-=(const U &other) { return *this -= Modular(other); }
    Modular &operator++() { return *this += 1; }
    Modular &operator--() { return *this -= 1; }
    Modular operator++(int)
    {
        Modular result(*this);
        *this += 1;
        return result;
    }
    Modular operator--(int)
    {
        Modular result(*this);
        *this -= 1;
        return result;
    }
    Modular operator-() const { return Modular(-value); }

    template <typename U = T>
    typename enable_if<is_same<typename Modular<U>::Type, int>::value, Modular>::type &operator*=(const Modular &rhs)
    {
        value = normalize(static_cast<int64_t>(value) * static_cast<int64_t>(rhs.value));
        return *this;
    }
    template <typename U = T>
    typename enable_if<is_same<typename Modular<U>::Type, int64_t>::value, Modular>::type &operator*=(const Modular &rhs)
    {
        int64_t q = int64_t(static_cast<long double>(value) * rhs.value / mod());
        value = normalize(value * rhs.value - q * mod());
        return *this;
    }
    template <typename U = T>
    typename enable_if<!is_integral<typename Modular<U>::Type>::value, Modular>::type &operator*=(const Modular &rhs)
    {
        value = normalize(value * rhs.value);
        return *this;
    }

    Modular &operator/=(const Modular &other) { return *this *= Modular(inverse(other.value, mod())); }

    friend const Type &abs(const Modular &x) { return x.value; }

    template <typename U>
    friend bool operator==(const Modular<U> &lhs, const Modular<U> &rhs);

    template <typename U>
    friend bool operator<(const Modular<U> &lhs, const Modular<U> &rhs);

    template <typename V, typename U>
    friend V &operator>>(V &stream, Modular<U> &number);

private:
    Type value;
};

template <typename T>
bool operator==(const Modular<T> &lhs, const Modular<T> &rhs) { return lhs.value == rhs.value; }
template <typename T, typename U>
bool operator==(const Modular<T> &lhs, U rhs) { return lhs == Modular<T>(rhs); }
template <typename T, typename U>
bool operator==(U lhs, const Modular<T> &rhs) { return Modular<T>(lhs) == rhs; }

template <typename T>
bool operator!=(const Modular<T> &lhs, const Modular<T> &rhs) { return !(lhs == rhs); }
template <typename T, typename U>
bool operator!=(const Modular<T> &lhs, U rhs) { return !(lhs == rhs); }
template <typename T, typename U>
bool operator!=(U lhs, const Modular<T> &rhs) { return !(lhs == rhs); }

template <typename T>
bool operator<(const Modular<T> &lhs, const Modular<T> &rhs) { return lhs.value < rhs.value; }

template <typename T>
Modular<T> operator+(const Modular<T> &lhs, const Modular<T> &rhs) { return Modular<T>(lhs) += rhs; }
template <typename T, typename U>
Modular<T> operator+(const Modular<T> &lhs, U rhs) { return Modular<T>(lhs) += rhs; }
template <typename T, typename U>
Modular<T> operator+(U lhs, const Modular<T> &rhs) { return Modular<T>(lhs) += rhs; }

template <typename T>
Modular<T> operator-(const Modular<T> &lhs, const Modular<T> &rhs) { return Modular<T>(lhs) -= rhs; }
template <typename T, typename U>
Modular<T> operator-(const Modular<T> &lhs, U rhs) { return Modular<T>(lhs) -= rhs; }
template <typename T, typename U>
Modular<T> operator-(U lhs, const Modular<T> &rhs) { return Modular<T>(lhs) -= rhs; }

template <typename T>
Modular<T> operator*(const Modular<T> &lhs, const Modular<T> &rhs) { return Modular<T>(lhs) *= rhs; }
template <typename T, typename U>
Modular<T> operator*(const Modular<T> &lhs, U rhs) { return Modular<T>(lhs) *= rhs; }
template <typename T, typename U>
Modular<T> operator*(U lhs, const Modular<T> &rhs) { return Modular<T>(lhs) *= rhs; }

template <typename T>
Modular<T> operator/(const Modular<T> &lhs, const Modular<T> &rhs) { return Modular<T>(lhs) /= rhs; }
template <typename T, typename U>
Modular<T> operator/(const Modular<T> &lhs, U rhs) { return Modular<T>(lhs) /= rhs; }
template <typename T, typename U>
Modular<T> operator/(U lhs, const Modular<T> &rhs) { return Modular<T>(lhs) /= rhs; }

template <typename T, typename U>
Modular<T> power(const Modular<T> &a, const U &b)
{
    assert(b >= 0);
    Modular<T> x = a, res = 1;
    U p = b;
    while (p > 0)
    {
        if (p & 1)
            res *= x;
        x *= x;
        p >>= 1;
    }
    return res;
}

template <typename T>
bool IsZero(const Modular<T> &number)
{
    return number() == 0;
}

template <typename T>
string to_string(const Modular<T> &number)
{
    return std::to_string(number());
}

// U == std::ostream? but done this way because of fastoutput
template <typename U, typename T>
U &operator<<(U &stream, const Modular<T> &number)
{
    return stream << number();
}

// U == std::istream? but done this way because of fastinput
template <typename U, typename T>
U &operator>>(U &stream, Modular<T> &number)
{
    typename common_type<typename Modular<T>::Type, int64_t>::type x;
    stream >> x;
    number.value = Modular<T>::normalize(x);
    return stream;
}

// using ModType = int; // Important for is_same<> to work

// struct VarMod
// {
//     static ModType value;
// };
// ModType VarMod::value;
// ModType &mod = VarMod::value;
// using Mint = Modular<VarMod>;

constexpr int mod = 1e9 + 7;
using Mint = Modular<std::integral_constant<decay<decltype(mod)>::type, mod>>;
Mint operator""_m(unsigned long long literal)
{
    return Mint(literal);
}

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
    Mint (&operator[](int row)) [Cols]
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

    // Calculates S = A^0 + A^1 + ... + A^k
    static Matrix<Rows, Cols> sumOfPowers(const Matrix<Rows, Cols> &A, ll k)
    {
        static_assert(Rows == Cols, "Must be square");
        // Create augmented matrix M of size 2*N x 2*N
        // M = | A  I |
        //     | 0  I |
        Matrix<2 * Rows, 2 * Cols> M; // Default 0

        for (int i = 0; i < Rows; ++i)
        {
            for (int j = 0; j < Cols; ++j)
                M[i][j] = A[i][j];     // Top-left: A
            M[i][i + Cols] = 1;        // Top-right: I
            M[i + Rows][i + Cols] = 1; // Bottom-right: I
        }

        // M^(k+1) gives Top-right block as sum(A^0...A^k)
        M = M.matrixPower(k + 1);

        Matrix<Rows, Cols> res;
        for (int i = 0; i < Rows; ++i)
        {
            for (int j = 0; j < Cols; ++j)
                res[i][j] = M[i][j + Cols];
        }
        return res;
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
constexpr int sz = 4;
using Matrix4by4 = Matrix<sz, sz>;

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
        const int V = 4;
        Matrix4by4 A;
        for (int i = 0; i < V; i++)
        {
            for (int j = 0; j < V; j++)
            {
                if (i != j)
                    A[i][j] = 1; // K4 adjacency matrix, no self-loops
            }
        }
        Matrix4by4 walkSum = Matrix4by4::sumOfPowers(A, N);
        cout << walkSum[0][0] - 1;
    }
    return 0;
}