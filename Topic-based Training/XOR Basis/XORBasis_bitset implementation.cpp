#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

constexpr int BITS = 64;
struct XORBasis
{
    using Mask = bitset<BITS>;
    array<Mask, BITS> basis{}; // basis[i] is 0 or has its highest bit at i
    int sz = 0;

    XORBasis(const Mask &x = {}) { insertVector(x); }

    bool insertVector(Mask x) // true if x was independent of the basis
    {
        for (int i = BITS; i-- && sz < BITS;)
        {
            if (x[i])
            {
                if (!basis[i][i])
                    return basis[i] = x, sz++, true;
                x ^= basis[i];
            }
        }
        return false;
    }

    bool canRepresent(Mask x) const
    {
        for (int i = BITS; i--;)
            if (x[i] && (x ^= basis[i])[i]) // still set means no pivot at i
                return 0;
        return 1;
    }

    void reduce() // RREF in place, span unchanged
    {
        for (int i = 0; i < BITS; i++)
        {
            if (basis[i][i])
                for (int j = basis[i]._Find_first(); j < i; j = basis[i]._Find_next(j))
                    basis[i] ^= basis[j];
        }
    }

    bool hasKth(uint64_t k) const { return sz >= numeric_limits<decltype(k)>::digits || !(k >> sz); }
    // k-th smallest element of the span, 0-indexed (k = 0 gives 0). Requires k < 2^sz.
    Mask kthSmallest(uint64_t k)
    {
        reduce();
        Mask r;
        for (int i = 0; i < BITS; i++)
            if (basis[i][i])
                r ^= (k & 1 ? basis[i] : Mask()), k >>= 1;
        return r;
    }

    Mask getMaxXor() const
    {
        Mask r;
        for (int i = BITS; i--;)
            if (!r[i])
                r ^= basis[i];
        return r;
    }

    XORBasis &operator+=(const XORBasis &o)
    {
        if (o.sz == BITS) // Full basis spans everything
            return *this = o;
        for (int i = 0; i < BITS && sz < BITS; i++)
            if (o.basis[i][i])
                insertVector(o.basis[i]);
        return *this;
    }
    friend XORBasis operator+(XORBasis a, const XORBasis &b) { return a += b; }

    void clear() { *this = {}; }
};

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifdef LOCAL
    freopen("input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif
    int t = 1;
    ll N, Q;
    cin >> t;
    XORBasis xb;
    while (t--)
    {
        cin >> N;
        xb.insertVector(N);
    }
    cout << xb.getMaxXor().to_ullong();
    return 0;
}