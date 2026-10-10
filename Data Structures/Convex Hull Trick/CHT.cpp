#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

/*
Dynamic Convex Hull Trick (upper envelope, max query)
Lines are kept sorted by slope, each one stores p = the last x where it is still optimal.
update(a, b) : insert y = a * x + b       O(log N) amortized
query(x)     : max over inserted lines    O(log N)
For min: insert (-a, -b) and negate the answer.
*/

struct LineContainer
{
private:
	struct Line
	{
		mutable ll a, b, p;
		bool operator<(const Line &o) const { return a < o.a; }
		bool operator<(ll x) const { return p < x; }
	};
	using It = multiset<Line, less<>>::iterator;
	static constexpr ll INF = LLONG_MAX;
	multiset<Line, less<>> hull;

	static ll fdiv(ll a, ll b) { return a / b - ((a ^ b) < 0 && a % b); }
	// Sets x->p (the point where y overtakes x), returns true if x is useless
	bool isect(It x, It y)
	{
		if (y == hull.end())
			return x->p = INF, false;
		if (x->a == y->a)
			x->p = x->b > y->b ? INF : -INF;
		else
			x->p = fdiv(y->b - x->b, x->a - y->a);
		return x->p >= y->p;
	}

public:
	void insert(ll a, ll b)
	{
		auto z = hull.insert({a, b, 0}), y = z++, x = y;
		while (isect(y, z))
			z = hull.erase(z);
		if (x != hull.begin() && isect(--x, y))
			isect(x, y = hull.erase(y));
		while ((y = x) != hull.begin() && (--x)->p >= y->p)
			isect(x, hull.erase(y));
	}
	ll query(ll x) const
	{
		assert(!hull.empty());
		auto it = hull.lower_bound(x);
		return it->a * x + it->b;
	}
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
		int n;
		cin >> n;
		LineContainer cht;
		while (n--)
		{
			int tp;
			ll a, b, x;
			cin >> tp;
			if (tp == 1)
				cin >> a >> b, cht.insert(a, b);
			else
				cin >> x, cout << cht.query(x) << endl;
		}
	}
	return 0;
}