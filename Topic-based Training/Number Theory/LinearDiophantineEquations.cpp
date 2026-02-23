#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

/*
Represents a *particular* solution to ax + by = c, along with the
step sizes that generate all other solutions.

All solutions are parameterized by an integer k:
  x(k) = x0 + k * stepX
  y(k) = y0 - k * stepY

Construction is only possible via DiophantineSolution::solve(), so the
object can never exist in an "unsolved" / uninitialized state.
*/
class DiophantineSolution
{
private:
    DiophantineSolution(ll a, ll b, ll g, ll x0, ll y0, ll c)
        : a_(a), b_(b), g_(g), x0_(x0), y0_(y0) {}

    // Extended Euclidean algorithm: returns gcd(a, b); sets x, y s.t. ax + by = gcd.
    static ll extGCD(ll a, ll b, ll &x, ll &y)
    {
        if (b == 0)
        {
            x = 1;
            y = 0;
            return a;
        }
        ll x1, y1;
        ll d = extGCD(b, a % b, x1, y1);
        x = y1;
        y = x1 - y1 * (a / b);
        return d;
    }

    static ll floorDiv(ll a, ll b) { return a / b - ((a ^ b) < 0 && a % b); }
    static ll ceilDiv(ll a, ll b) { return a / b + ((a ^ b) > 0 && a % b); }
    ll a_, b_, g_;
    ll x0_, y0_;

public:
    // Attempts to solve ax + by = c.
    // Returns nullopt if no integer solution exists.
    static std::optional<DiophantineSolution> solve(ll a, ll b, ll c)
    {
        // Degenerate: 0x + 0y = c
        if (a == 0 && b == 0)
        {
            if (c != 0)
                return std::nullopt;
            // Infinite solutions – represent with a sentinel.
            return DiophantineSolution{0, 0, 0, 0, 0, 0};
        }

        ll x0, y0;
        ll g = extGCD(std::abs(a), std::abs(b), x0, y0);

        if (c % g != 0)
            return std::nullopt;

        x0 *= c / g;
        y0 *= c / g;
        if (a < 0)
            x0 = -x0;
        if (b < 0)
            y0 = -y0;

        return DiophantineSolution{a, b, g, x0, y0, c};
    }

    // Returns the solution {x(k), y(k)} for a given k.
    std::pair<ll, ll> at(ll k) const { return {x0_ + k * stepX(), y0_ - k * stepY()}; }

    // Counts integer solutions with x in [minX, maxX] and y in [minY, maxY].
    // Returns -1 to signal "infinitely many" (only when a == b == 0, c == 0).
    ll countInBox(ll minX, ll maxX, ll minY, ll maxY) const
    {
        // Degenerate sentinel: a == b == 0, c == 0 → infinite solutions.
        if (a_ == 0 && b_ == 0)
            return -1;

        // a == 0: y is fixed, x ranges freely.
        if (a_ == 0)
            return (minY <= y0_ && y0_ <= maxY) ? (maxX - minX + 1) : 0;

        // b == 0: x is fixed, y ranges freely.
        if (b_ == 0)
            return (minX <= x0_ && x0_ <= maxX) ? (maxY - minY + 1) : 0;

        // General case: intersect the k-ranges imposed by each bound.
        ll dx = stepX();
        ll dy = stepY();

        ll kMinX = (dx > 0) ? ceilDiv(minX - x0_, dx) : ceilDiv(maxX - x0_, dx);
        ll kMaxX = (dx > 0) ? floorDiv(maxX - x0_, dx) : floorDiv(minX - x0_, dx);

        ll kMinY = (dy > 0) ? ceilDiv(y0_ - maxY, dy) : ceilDiv(y0_ - minY, dy);
        ll kMaxY = (dy > 0) ? floorDiv(y0_ - minY, dy) : floorDiv(y0_ - maxY, dy);

        ll kStart = std::max(kMinX, kMinY);
        ll kEnd = std::min(kMaxX, kMaxY);

        return (kStart > kEnd) ? 0 : (kEnd - kStart + 1);
    }

    // Does any solution land inside the box?
    bool hasInBox(ll minX, ll maxX, ll minY, ll maxY) const { return countInBox(minX, maxX, minY, maxY) > 0; }

    // Accessors for the particular solution and step sizes.
    ll x0() const { return x0_; }
    ll y0() const { return y0_; }
    ll stepX() const { return b_ / g_; } // Δx per unit k
    ll stepY() const { return a_ / g_; } // Δy per unit k
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
    // cin >> t;
    while (t--)
    {
        ll a, b, c;
        cin >> a >> b >> c;
        auto solution = DiophantineSolution::solve(a, b, c);
        if (solution.has_value() && solution->hasInBox(0, 10000, 0, 10000))
            cout << "Yes";
        else
            cout << "No";
    }
    return 0;
}