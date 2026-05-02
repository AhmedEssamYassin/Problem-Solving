#include <bits/stdc++.h>
#pragma GCC optimize("Ofast,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
using namespace std;
#define ll long long int
#define endl "\n"

// Fast I/O for 128-bit integers
using int128 = signed __int128;
using uint128 = unsigned __int128;
ostream &operator<<(ostream &stream, int128 n)
{
    if (__builtin_expect(n == 0, 0))
        return stream.put('0');
    char buf[41];
    char *end = buf + 41;
    char *p = end;
    bool neg = (n < 0);
    uint128 u = neg ? -(uint128)n : (uint128)n;
    do
    {
        *--p = (char)('0' + (unsigned)(u % 10));
        u /= 10;
    } while (u);
    if (neg)
        *--p = '-';
    return stream.write(p, end - p);
}

ostream &operator<<(ostream &stream, uint128 n)
{
    if (__builtin_expect(n == 0, 0))
        return stream.put('0');
    char buf[40];
    char *end = buf + 40;
    char *p = end;
    do
    {
        *--p = (char)('0' + (unsigned)(n % 10));
        n /= 10;
    } while (n);
    return stream.write(p, end - p);
}

istream &operator>>(istream &stream, int128 &n)
{
    n = 0;
    char c;
    if (!(stream >> c))
        return stream;
    bool neg = (c == '-');
    if (neg && !stream.get(c))
        return stream;
    while (isdigit((unsigned char)c))
    {
        n = n * 10 + (c - '0');
        if (!stream.get(c))
            break;
    }
    if (stream)
        stream.putback(c);
    if (neg)
        n = -n;
    return stream;
}

istream &operator>>(istream &stream, uint128 &n)
{
    n = 0;
    char c;
    if (!(stream >> c))
        return stream;
    while (isdigit((unsigned char)c))
    {
        n = n * 10 + (c - '0');
        if (!stream.get(c))
            break;
    }
    if (stream)
        stream.putback(c);
    return stream;
}

// Use type u128
namespace Montgomery128
{
    using u128 = __uint128_t;
    // ---------- 64-bit limb helpers (fast) ----------
    static inline void mult64(uint64_t a, uint64_t b, uint64_t &lo, uint64_t &hi)
    {
        u128 r = (u128)a * b;
        lo = (uint64_t)r;
        hi = (uint64_t)(r >> 64);
    }
    static inline uint64_t add64(uint64_t a, uint64_t b, uint64_t &carry)
    {
        u128 r = (u128)a + b + carry;
        carry = (uint64_t)(r >> 64);
        return (uint64_t)r;
    }
    // Fallback generic (slow): kept for setup (e.g., computing R^2 mod N once)
    u128 mult128(u128 a, u128 b, u128 mod)
    {
        u128 result = 0;
        for (a %= mod; b > 0; a <<= 1, b >>= 1)
        {
            a >= mod ? a -= mod : 0;
            if (b & 1)
                result += a, result >= mod ? result -= mod : 0;
        }
        return result;
    }
    // ---------- Montgomery (CIOS, 2x64-bit limbs) ----------
    // Compute n0' = -N^{-1} mod 2^64 (low limb only)
    static inline uint64_t inv64_2k(uint64_t n0)
    {
        uint64_t x = 1; // initial approx
        for (int i = 6; i > 0; --i)
            x = (__uint128_t)x * (2 - (__uint128_t)n0 * x);
        return x; // x ≡ n0^{-1}
    }

    inline pair<u128, u128> montModInv(u128 N) { return {0, (u128)(0 - inv64_2k(N))}; }

    // Fast Montgomery multiplication: returns a*b*R^{-1} mod N, where R=2^128
    inline u128 montMult(u128 a, u128 b, u128 N, u128 N_n0prime)
    {
        uint64_t n0 = (uint64_t)N, n1 = (uint64_t)(N >> 64);
        uint64_t a0 = (uint64_t)a, a1 = (uint64_t)(a >> 64);
        uint64_t b0 = (uint64_t)b, b1 = (uint64_t)(b >> 64);
        uint64_t n0p = (uint64_t)N_n0prime;
        uint64_t t0 = 0, t1 = 0, t2 = 0, t3 = 0;

        auto roundStep = [&](uint64_t ai) __attribute__((always_inline))
        {
            uint64_t lo, hi, carry = 0;
            // t += ai * b
            mult64(ai, b0, lo, hi);
            t0 = add64(t0, lo, carry);
            t1 = add64(t1, hi, carry);
            t2 = add64(t2, 0, carry);
            mult64(ai, b1, lo, hi);
            carry = 0;
            t1 = add64(t1, lo, carry);
            t2 = add64(t2, hi, carry);
            t3 = add64(t3, 0, carry);
            // m = t0 * n0' (mod 2^64)
            uint64_t m = (uint64_t)((__uint128_t)t0 * n0p);
            // t += m * N
            mult64(m, n0, lo, hi);
            carry = (t0 != 0); // Mathematically exact replacement for t0 = add64(t0, lo, carry);
            t1 = add64(t1, hi, carry);
            t2 = add64(t2, 0, carry);
            t3 = add64(t3, 0, carry);

            mult64(m, n1, lo, hi);
            carry = 0;
            t1 = add64(t1, lo, carry);
            t2 = add64(t2, hi, carry);
            t3 = add64(t3, 0, carry);

            // shift by one limb
            t0 = t1;
            t1 = t2;
            t2 = t3;
            t3 = 0;
        };

        roundStep(a0);
        roundStep(a1);

        __uint128_t res = (((__uint128_t)t1 << 64) | t0);
        if (t2 || res >= N)
            res -= N;
        return (u128)res;
    }
}
using namespace Montgomery128;

template <typename T>
inline T absVal(T N) { return N < 0 ? -N : N; }

constexpr inline uint32_t bitWidth128(u128 n)
{
    if (!n)
        return 0;
    uint64_t hi = (uint64_t)(n >> 64);
    uint64_t lo = (uint64_t)n;
    return hi ? 128 - countl_zero(hi) : 64 - countl_zero(lo);
}

constexpr inline int ctz128(u128 x)
{
    if (!x)
        return 128;
    uint64_t hi = (uint64_t)(x >> 64);
    uint64_t lo = (uint64_t)x;
    return lo ? countr_zero(lo) : 64 + countr_zero(hi);
}

template <typename T>
T GCD(T a, T b)
{
    if (!a || !b)
        return a | b;
    int shift = ctz128(a | b);
    a >>= ctz128(a);
    do
    {
        b >>= ctz128(b);
        if (a > b)
            swap(a, b);
        b -= a;
    } while (b);
    return a << shift;
}

template <typename T>
inline T F(T x, T c, T mod, T inv) // Pollard-Rho function
{
    x = montMult(x, x, mod, inv);
    x = x >= mod - c ? x - mod + c : x + c;
    return x;
}

template <typename T>
T pollardBrent(T N)
{
    if (!(N & 1))
        return 2;

    // Random Number Linear Congruential Generator MMIX from D.E. Knuth
    static u128 rng = 0xdeafbeefff;
    uint64_t a = rng * 6364136223846793005ULL + 1442695040888963407ULL;
    uint64_t b = a * 6364136223846793005ULL + 1442695040888963407ULL;
    rng = (a + b) ^ (a * b);

    T X0 = 1 + a % (N - 1);
    T C = 1 + b % (N - 1);
    T X = X0; // X1
    T gcdVal = 1;
    T q = 1;
    T Xs, Xt;
    T m = 128;
    u128 inv = montModInv(N).second;
    T L = 1;
    while (gcdVal == 1)
    {
        Xt = X;
        for (size_t i = 1; i < L; ++i)
            X = F(X, C, N, inv);

        uint64_t k = 0;
        while (k < L && gcdVal == 1)
        {
            Xs = X;
            for (size_t i = 0; i < m && i + k < L; ++i)
            {
                X = F(X, C, N, inv);
                q = montMult(q, Xt > X ? Xt - X : X - Xt, N, inv);
            }
            gcdVal = GCD(q, N);
            k += m;
        }
        L += L;
    }
    if (gcdVal == N) // Failure
    {
        do
        {
            Xs = F(Xs, C, N, inv);
            gcdVal = GCD(Xs > Xt ? Xs - Xt : Xt - Xs, N);
        } while (gcdVal == 1);
    }
    return gcdVal;
}

template <typename T>
T modPow(T N, T power, T mod)
{
    if (N % mod == 0 || N == 0)
        return 0;
    if (N == 1 || power == 0)
        return 1;
    T res{1};
    while (power)
    {
        if (power & 1)
            res = mult128(res, N, mod);
        N = mult128(N, N, mod);
        power >>= 1;
    }
    return res;
}

template <typename T>
bool millerRabin(T N)
{
    T d = N - 1;
    int s{};
    while (!(d & 1))
        d >>= 1, ++s;

    constexpr int a = 2;
    T p = modPow(a % N, d, N), i = s;
    while (p != 1 && p != N - 1 && a % N && i--)
        p = mult128(p, p, N);
    if (p != N - 1 && i != s)
        return false;
    return true;
}

template <typename T>
int jacobi(T D, T n)
{
    int t = 1;
    D %= n;
    while (D != 0)
    {
        int r = 0;
        while (!(D & 1))
            D >>= 1, r++;
        if (r & 1)
        {
            T nm = n & 7;
            if (nm == 3 || nm == 5)
                t = -t;
        }
        swap(D, n);

        if ((D & 3) == 3 && (n & 3) == 3)
            t = -t;
        D %= n;
    }
    return (n == 1) ? t : 0;
}

constexpr u128 iSqrt(u128 n) // O(log(log(n)))
{
    if (n < 2)
        return n;
    u128 xk = (u128)1 << ((bitWidth128(n) + 1) >> 1);
    u128 nxt = (xk + n / xk) >> 1;
    while (nxt < xk)
    {
        xk = nxt;
        nxt = (xk + n / xk) >> 1;
    }
    return xk;
}

template <typename T>
bool isPerfectSquare(T n)
{
    if (n == 0 || n == 1)
        return true;

    static constexpr unsigned long long MASK =
        0x0202021202030213ULL;

    if (((MASK >> (n & 63)) & 1) == 0)
        return false;

    T r = iSqrt(n);
    return r * r == n;
}

void calcLucas(u128 k, u128 n, ll d, ll p, ll q, u128 &U, u128 &V, u128 &Qk)
{
    if (k == 0)
    {
        U = 0;
        V = 2;
        Qk = 1;
        return;
    }

    auto add = [&](u128 a, u128 b)
    { return (a + b >= n) ? a + b - n : a + b; };
    auto sub = [&](u128 a, u128 b)
    { return (a >= b) ? a - b : a + n - b; };
    auto div2 = [&](u128 a)
    { return (a & 1) ? (a + n) >> 1 : a >> 1; };

    u128 P = (p < 0) ? n - (u128)(-p) % n : (u128)p % n;
    u128 Q = (q < 0) ? n - (u128)(-q) % n : (u128)q % n;
    u128 D = (d < 0) ? n - (u128)(-d) % n : (u128)d % n;

    int bits = bitWidth128(k);
    U = 1;
    V = P;
    Qk = Q;

    for (int i = bits - 2; i >= 0; i--)
    {
        U = mult128(U, V, n);
        V = sub(mult128(V, V, n), add(Qk, Qk));
        Qk = mult128(Qk, Qk, n);

        if ((k >> i) & 1)
        {
            u128 nU = div2(add(mult128(P, U, n), V));
            u128 nV = div2(add(mult128(D, U, n), mult128(P, V, n)));
            U = nU;
            V = nV;
            Qk = mult128(Qk, Q, n);
        }
    }
}

bool strongLucasSelfridge(u128 n)
{
    ll D = 5, sign = 1;
    while (true)
    {
        ll curD = D * sign;
        u128 jacD = (curD < 0) ? (n - (u128)(-curD) % n) % n : ((u128)curD % n);
        int j = jacobi(jacD, n);
        if (j == -1)
        {
            D = curD;
            break;
        }

        if (j == 0)
            return n == (u128)(curD < 0 ? -curD : curD);
        D += 2;
        sign = -sign;
    }

    ll P = 1;
    ll Q = (1 - D) / 4;

    if (P == 1 && Q == -1)
        P = 5, Q = 5;

    u128 d = n + 1;
    int s = 0;
    while (!(d & 1))
        d >>= 1, s++;

    u128 U, V, Qk;
    calcLucas(d, n, D, P, Q, U, V, Qk);

    bool isSlprp = (U == 0 || V == 0);

    auto add = [&](u128 a, u128 b)
    { return (a + b >= n) ? a + b - n : a + b; };
    auto sub = [&](u128 a, u128 b)
    { return (a >= b) ? a - b : a + n - b; };

    for (int r = 1; r < s; r++)
    {
        V = sub(mult128(V, V, n), add(Qk, Qk));
        Qk = mult128(Qk, Qk, n);
        if (V == 0)
            isSlprp = true;
    }
    if (!isSlprp)
        return false;
    u128 vNext = sub(mult128(V, V, n), add(Qk, Qk));
    u128 expectedV = (Q < 0) ? (n - ((u128)(-(int128)Q) * 2) % n) % n : ((u128)Q * 2) % n;
    if (vNext != expectedV)
        return false;

    u128 qMod = (Q < 0) ? (n - (u128)(-Q) % n) % n : (u128)Q % n;
    int jacQ = jacobi(qMod, n);
    u128 expectedQ = (jacQ == -1) ? n - qMod : qMod;
    if (Qk != expectedQ)
        return false;

    return true;
}

template <typename T>
bool isBPSWPrime(T n)
{
    if (n < 64)
    {
        constexpr uint64_t MASK = 0x28208A20A08A28ACULL;
        return (MASK >> n) & 1;
    }

    uint32_t x = (uint32_t)(n % 30);
    constexpr uint32_t WHEEL30 = 0x208A2882;

    if (!((WHEEL30 >> x) & 1))
        return false;

    if (!millerRabin(n))
        return false;

    if (isPerfectSquare(n))
        return false;

    return strongLucasSelfridge(n);
}

template <typename T>
void primeFactorize(T N, vector<T> &primeFactors)
{
    if (N == 1)
        return;

    if (isBPSWPrime(N))
    {
        primeFactors.push_back(N);
        return;
    }
    T Y = pollardBrent(N);
    primeFactorize(Y, primeFactors);
    primeFactorize(N / Y, primeFactors);
}

template <typename T>
T countDivisors(T N)
{
    vector<T> primeFactors;
    primeFactorize(N, primeFactors);
    sort(primeFactors.begin(), primeFactors.end());

    T ans = 1;
    for (size_t i = 0, sz = primeFactors.size(); i < sz;)
    {
        size_t j = i;
        while (j < sz && primeFactors[i] == primeFactors[j])
            ++j;
        ans *= (j - i + 1);
        i = j;
    }
    return ans;
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
        u128 N;
        cin >> N;
        u128 x = 1;
        vector<u128> pf;
        while (N--)
        {
            u128 a;
            cin >> a;
            primeFactorize(a, pf);
        }
        sort(pf.begin(), pf.end());
        for (size_t i = 0, sz = pf.size(); i < sz;)
        {
            size_t j = i;
            while (j < sz && pf[i] == pf[j])
                ++j;
            (x *= (j - i + 1)) %= 998244353;
            i = j;
        }
        cout << x << endl;
    }
    return 0;
}