#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

/*
A heavy child of a node is the child with the largest subtree size rooted at the child.
A light child of a node is any child that is not a heavy child.
A heavy edge connects a node to its heavy child.
A light edge connects a node to any of its light children.
A heavy path is the path formed by a collection heavy edges.
A light path is the path formed by a collection light edges.
*/

constexpr int BITS = 20;
struct XORBasis
{
	using word = conditional_t<(BITS > 32), uint64_t, uint32_t>;
	array<word, BITS> basis{}; // basis[i] is 0 or has its highest bit at i
	int sz = 0;

	XORBasis(word x = 0) { insertVector(x); }

	bool insertVector(word x) // true if x was independent of the basis
	{
		for (int i = 0; x && sz < BITS; x ^= basis[i])
			if (!basis[i = __lg(x)])
				return basis[i] = x, sz++, true;
		return false;
	}

	bool canRepresent(word x) const
	{
		for (int i = 0; x; x ^= basis[i])
			if (!basis[i = __lg(x)])
				return 0;
		return 1;
	}

	word getMaxXor() const
	{
		word r = 0;
		for (int i = BITS; i--;)
			r = max(r, r ^ basis[i]);
		return r;
	}

	XORBasis &operator+=(const XORBasis &o)
	{
		if (o.sz == BITS) // Full basis spans everything
			return *this = o;
		for (word v : o.basis)
			insertVector(v);
		return *this;
	}
	friend XORBasis operator+(XORBasis a, const XORBasis &b) { return a += b; }

	void clear() { *this = {}; }
};

template <typename T, typename F>
struct SparseTable
{
	vector<vector<T>> t; // t[k][i] = f over a[i..i + 2^k - 1]
	F f;
	T id; // Returned for an empty range: min -> INF, max -> -INF, gcd / OR -> 0, & -> -1LL, lcm -> 1
	SparseTable() = default;
	SparseTable(const vector<T> &a, F f, T id = T()) : t{a}, f(f), id(id)
	{
		for (int k = 1; (1 << k) <= (int)a.size(); k++)
		{
			t.emplace_back(a.size() - (1 << k) + 1);
			for (int i = 0; i < (int)t[k].size(); i++)
				t[k][i] = f(t[k - 1][i], t[k - 1][i + (1 << (k - 1))]);
		}
	}

	T query(int L, int R) const // f over a[L..R], 0 <= L <= R < n
	{
		if (L > R)
			return id;
		assert(0 <= L && R < (int)t[0].size());
		int k = __lg(R - L + 1);
		return f(t[k][L], t[k][R - (1 << k) + 1]);
	}
};

struct HeavyLightDecomposition
{
private:
	int timer = 0;
	vector<int> parent, depth, heavy, head, in, out, size;
	SparseTable<XORBasis, std::function<XORBasis(const XORBasis &, const XORBasis &)>> SPT;

	int dfsSize(const vector<vector<ll>> &Tree, int u)
	{
		size[u] = 1;
		int maxSubtree = 0;
		for (const auto &v : Tree[u])
		{
			if (v == parent[u])
				continue;
			parent[v] = u;
			depth[v] = depth[u] + 1;
			size[u] += dfsSize(Tree, v);
			if (size[v] > maxSubtree)
			{
				heavy[u] = v;
				maxSubtree = size[v];
			}
		}
		return size[u];
	}

	void dfsHld(const vector<vector<ll>> &Tree, const vector<ll> &values, vector<XORBasis> &baseArray, int u, int h)
	{
		head[u] = h;
		in[u] = timer++;
		baseArray[in[u]] = values[u];
		if (heavy[u] != -1)
			dfsHld(Tree, values, baseArray, heavy[u], h);
		for (const auto &v : Tree[u])
		{
			if (v != parent[u] && v != heavy[u])
				dfsHld(Tree, values, baseArray, v, v);
		}
		out[u] = timer - 1;
	}

public:
	HeavyLightDecomposition(const vector<vector<ll>> &Tree, int root, const vector<ll> &values)
	{
		int N = Tree.size();
		parent.assign(N, -1);
		depth.resize(N);
		heavy.assign(N, -1);
		head.resize(N);
		in.resize(N);
		out.resize(N);
		size.resize(N);

		dfsSize(Tree, root);
		vector<XORBasis> baseArray(N);
		dfsHld(Tree, values, baseArray, root, 0);
		SPT = decltype(SPT)(baseArray, [](const XORBasis &x, const XORBasis &y)
							{ return (x + y); }, XORBasis());
	}

	ll getDepth(ll u) const
	{
		return depth[u];
	}

	bool queryPath(int u, int v, int k)
	{
		XORBasis res;
		while (head[u] != head[v])
		{
			if (depth[head[u]] < depth[head[v]])
				swap(u, v);
			res = res + SPT.query(in[head[u]], in[u]);
			u = parent[head[u]];
		}
		if (depth[u] > depth[v])
			swap(u, v);
		res = res + SPT.query(in[u], in[v]);
		return res.canRepresent(k);
	}

	XORBasis querySubtree(int u)
	{
		return SPT.query(in[u], out[u]);
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
	ll N, Q;
	// cin >> t;
	while (t--)
	{
		cin >> N;
		vector<vector<ll>> Tree(N + 1);
		vector<ll> val(N + 1);
		for (int u = 1; u <= N; u++)
			cin >> val[u];
		ll anyNode = 1;
		for (int i{}; i < N - 1; i++)
		{
			ll u, v;
			cin >> u >> v;
			anyNode = u;
			Tree[u].push_back(v);
			Tree[v].push_back(u);
		}
		ll root = anyNode;
		HeavyLightDecomposition hld(Tree, root, val);

		cin >> Q;
		while (Q--)
		{
			ll x, y, k;
			cin >> x >> y >> k;
			cout << (hld.queryPath(x, y, k) ? "YES" : "NO") << endl;
		}
	}
	return 0;
}