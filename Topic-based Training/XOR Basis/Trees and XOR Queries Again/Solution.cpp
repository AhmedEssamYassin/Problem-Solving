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

const int BITS = 20;
#define int_type std::conditional_t<(BITS > 31), long long, int>
struct XORBasis
{
	int sz = 0;
	array<int_type, BITS> basis{};
	XORBasis() {}
	XORBasis(const ll &x)
	{
		insertVector(x);
	}
	void insertVector(ll x)
	{
		if (sz == BITS)
			return;
		for (ll i = __lg(x); x > 0; i = __lg(x))
		{
			if (!basis[i])
				return sz++, void(basis[i] = x);
			x ^= basis[i];
		}
	}

	bool canRepresent(ll x)
	{
		if (sz == BITS)
			return true;
		for (ll i = __lg(x); x > 0; i = __lg(x))
		{
			if (basis[i])
				x ^= basis[i];
			else
				return false;
		}
		return !x;
	}

	ll getMaxXor()
	{
		if (sz == BITS)
			return (1LL << BITS) - 1;
		ll maxXor = 0;
		for (int i = BITS - 1; i >= 0; i--)
		{
			if ((maxXor ^ basis[i]) > maxXor)
				maxXor ^= basis[i];
		}
		return maxXor;
	}

	friend XORBasis operator+(const XORBasis &LHS, const XORBasis &RHS)
	{
		XORBasis res;
		if (LHS.sz == BITS)
			return LHS;

		if (RHS.sz == BITS)
			return (RHS);
		res = LHS;
		for (int i = 0; i < BITS; i++)
		{
			if (RHS.basis[i])
				res.insertVector(RHS.basis[i]);
		}
		return res;
	}

	XORBasis &operator+=(const XORBasis &other)
	{
		if (sz == BITS)
			return *this;

		if (other.sz == BITS)
			return (*this = other);

		for (int i = 0; i < BITS; i++)
		{
			if (other.basis[i])
				insertVector(other.basis[i]);
		}
		return *this;
	}

	void clear()
	{
		if (!sz)
			return;
		basis.fill(0);
		sz = 0;
	}
};

ll bitCeil(const ll &N)
{
	ll res{1};
	while (res < N)
		res <<= 1;
	return res;
}

struct SparseTable
{
	int size, LOG;
	vector<vector<XORBasis>> m;
	XORBasis merge(const XORBasis &a, const XORBasis &b)
	{
		return (a + b);
	}
	void build(const vector<ll> &arr)
	{
		int N = arr.size();
		for (int i{}; i < N; i++)
			m[i][0] = arr[i];
		for (int k = 1; k < LOG; k++)
		{
			for (int i{}; i + (1 << k) - 1 < N; i++)
				m[i][k] = merge(m[i][k - 1], m[i + (1 << (k - 1))][k - 1]);
		}
	}
	SparseTable() {}
	SparseTable(const vector<ll> &arr)
	{
		int n = arr.size();
		LOG = (ll)(log2l(bitCeil(n)) + 1) + 1;
		m.resize(n, vector<XORBasis>(LOG, 0));
		build(arr);
	}

	XORBasis query(int L, int R) // 0-based
	{
		int len = R - L + 1;
		int k = 31 - __builtin_clz(len);
		return merge(m[L][k], m[R - (1 << k) + 1][k]);
	}
};

ll Min(ll a, ll b)
{
	return std::min(a, b);
}

struct HeavyLightDecomposition
{
private:
	int timer = 0;
	vector<int> parent, depth, heavy, head, in, out, size;
	SparseTable SPT;

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

	void dfsHld(const vector<vector<ll>> &Tree, const vector<ll> &values, vector<ll> &baseArray, int u, int h)
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
		vector<ll> baseArray(N);
		dfsHld(Tree, values, baseArray, root, 0);
		SPT = SparseTable(baseArray);
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
			res = SPT.merge(res, SPT.query(in[head[u]], in[u]));
			u = parent[head[u]];
		}
		if (depth[u] > depth[v])
			swap(u, v);
		res = SPT.merge(res, SPT.query(in[u], in[v]));
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
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
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