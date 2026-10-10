#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct Point
{
	ll x, y;
	Point operator-(const Point &o) const { return {x - o.x, y - o.y}; }
	ll cross(const Point &o) const { return x * o.y - y * o.x; }
	ll dot(const Point &o) const { return x * o.x + y * o.y; }
	ll norm2() const { return dot(*this); }
	bool operator==(const Point &o) const { return x == o.x && y == o.y; }
	bool operator<(const Point &o) const { return x != o.x ? x < o.x : y < o.y; }
};
ll cross3(const Point &O, const Point &A, const Point &B) { return (A - O).cross(B - O); }
bool onSegment(const Point &a, const Point &b, const Point &p)
{
	return !cross3(a, b, p) && min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) && min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
}

struct ConvexHull
{
	vector<Point> h; // CCW, h[0] = lexicographically smallest
	int R = 0;       // index of lexicographically largest point
	bool kc = 0;     // built with keepCollinear -> enumeration only, queries assert
	void build(vector<Point> p, bool keep = 0)
	{
		kc = keep;
		R = 0;
		h.clear();
		sort(p.begin(), p.end());
		p.erase(unique(p.begin(), p.end()), p.end());
		int n = p.size(), k = 0;
		if (n <= 2)
		{
			h = p;
			R = max(n - 1, 0);
			return;
		}
		if (all_of(p.begin(), p.end(), [&](const Point &q)
		           { return !cross3(p[0], p.back(), q); }))
		{
			h = keep ? p : vector<Point>{p[0], p.back()};
			R = h.size() - 1;
			return;
		}
		h.resize(2 * n);
		auto bad = [&](ll c)
		{ return keep ? c < 0 : c <= 0; };
		for (int i = 0; i < n; i++)
		{
			while (k >= 2 && bad(cross3(h[k - 2], h[k - 1], p[i])))
				k--;
			h[k++] = p[i];
		}
		for (int i = n - 2, s = k + 1; i >= 0; i--)
		{
			while (k >= s && bad(cross3(h[k - 2], h[k - 1], p[i])))
				k--;
			h[k++] = p[i];
		}
		h.resize(k - 1);
		for (int i = 1; i < k - 1; i++)
			if (h[R] < h[i])
				R = i;
	}
	int size() const { return h.size(); }
	ll area2() const
	{
		ll a = 0;
		for (int i = 1; i + 1 < (int)h.size(); i++)
			a += cross3(h[0], h[i], h[i + 1]);
		return a;
	}
	double area() const { return llabs(area2()) / 2.0; }
	double perimeter() const
	{
		int n = h.size();
		long double s = 0;
		if (n < 2)
			return 0;
		for (int i = 0; i < n; i++)
			s += sqrtl((h[(i + 1) % n] - h[i]).norm2());
		return s;
	}

	// 1 inside, 0 boundary, -1 outside; O(log N)
	int locate(const Point &p) const
	{
		assert(!kc);
		int n = h.size();
		if (n < 3)
			return n && onSegment(h[0], h[n - 1], p) ? 0 : -1;
		ll c1 = cross3(h[0], h[1], p), c2 = cross3(h[0], h[n - 1], p);
		if (c1 < 0 || c2 > 0)
			return -1;
		if (!c1)
			return onSegment(h[0], h[1], p) ? 0 : -1;
		if (!c2)
			return onSegment(h[0], h[n - 1], p) ? 0 : -1;
		int lo = 1, hi = n - 1;
		while (hi - lo > 1)
		{
			int m = (lo + hi) / 2;
			(cross3(h[0], h[m], p) > 0 ? lo : hi) = m;
		}
		ll c = cross3(h[lo], h[hi], p);
		return c < 0 ? -1 : c > 0 ? 1
		                          : 0;
	}
	bool contains(const Point &p) const { return locate(p) >= 0; }
	bool strictlyInside(const Point &p) const { return locate(p) == 1; }
	bool onBoundary(const Point &p) const { return locate(p) == 0; }

	// vertex maximizing dot(h[i], d); O(log N)
	int extremeIndex(const Point &d) const
	{
		assert(!kc);
		int n = h.size();
		if (n == 1)
			return 0;
		auto go = [&](int s, int len)
		{ int lo = 0, hi = len - 1;
            while (lo < hi) { int m = (lo + hi) / 2; if (h[(s + m) % n].dot(d) < h[(s + m + 1) % n].dot(d)) lo = m + 1; else hi = m; }
            return (s + lo) % n; };
		int a = go(0, R + 1), b = go(R, n - R + 1);
		return h[a].dot(d) >= h[b].dot(d) ? a : b;
	}
	Point extremePoint(const Point &d) const { return h[extremeIndex(d)]; }

	// rotating calipers: f(i, j) = antipodal vertex j for edge (i, i+1); needs n >= 2
	template <class F>
	void calipers(F f) const
	{
		int n = h.size();
		for (int i = 0, j = 1; i < n; i++)
		{
			const Point &a = h[i], &b = h[(i + 1) % n];
			while (llabs(cross3(a, b, h[(j + 1) % n])) > llabs(cross3(a, b, h[j])))
				j = (j + 1) % n;
			f(i, j);
		}
	}
	tuple<ll, int, int> diameter() const
	{ // {max dist^2, i, j}
		assert(!kc);
		int n = h.size();
		tuple<ll, int, int> r{0, 0, 0};
		if (n > 1)
			calipers([&](int i, int j)
			         { for (int a : {i, (i + 1) % n}) { ll d = (h[a] - h[j]).norm2(); if (d > get<0>(r)) r = {d, a, j}; } });
		return r;
	}
	double width() const
	{
		assert(!kc);
		int n = h.size();
		if (n <= 2)
			return 0;
		double b = 1e18;
		calipers([&](int i, int j)
		         { const Point &a = h[i], &c = h[(i + 1) % n];
            b = min(b, (double)(llabs(cross3(a, c, h[j])) / sqrtl((c - a).norm2()))); });
		return b;
	}
	// tangent vertex indices {right, left} from external point p; O(N)
	pair<int, int> tangentsFromPoint(const Point &p) const
	{
		assert(!kc);
		int n = h.size(), a = -1, b = -1;
		for (int i = 0; i < n; i++)
		{
			ll c1 = cross3(p, h[i], h[(i + 1) % n]), c2 = cross3(p, h[(i + n - 1) % n], h[i]);
			if (c1 >= 0 && c2 <= 0)
				a = i;
			if (c1 <= 0 && c2 >= 0)
				b = i;
		}
		return {a, b};
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
		int s, r, m;
		cin >> s >> r >> m;
		vector<Point> Sh(s), R(r), M(m);
		for (auto &[x, y] : Sh)
			cin >> x >> y;
		for (auto &[x, y] : R)
			cin >> x >> y;
		for (auto &[x, y] : M)
			cin >> x >> y;
		ConvexHull Sheena, Rose, Maria;
		Sheena.build(Sh);
		Rose.build(R);
		Maria.build(M);
		int n;
		cin >> n;
		for (int i{}; i < n; i++)
		{
			Point p;
			cin >> p.x >> p.y;
			if (Sheena.contains(p))
				cout << "Sheena\n";
			else if (Rose.contains(p))
				cout << "Rose\n";
			else if (Maria.contains(p))
				cout << "Maria\n";
			else
				cout << "Outside\n";
		}
	}
	return 0;
}
