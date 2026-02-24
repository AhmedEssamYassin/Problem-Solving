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

struct SuccinctBitVector
{
	int n, blocks;
	vector<uint64_t> block;
	vector<int> pref;

	SuccinctBitVector() {}

	SuccinctBitVector(int size)
	{
		n = size;
		blocks = (n >> 6) + 1;
		block.assign(blocks, 0);
		pref.assign(blocks, 0);
	}

	void setBit(int i) { block[i >> 6] |= (1ULL << (i & 63)); }

	void build()
	{
		for (int i = 0; i < blocks - 1; i++)
			pref[i + 1] = pref[i] + __builtin_popcountll(block[i]);
	}

	int rank1(int i) { return pref[i >> 6] + __builtin_popcountll(block[i >> 6] & ((1ULL << (i & 63)) - 1)); }

	int rank0(int i) { return i - rank1(i); }
};

class CompressedRange
{
private:
	vector<ll> init;
	void compress(vector<ll> &vec)
	{
		sort(vec.begin(), vec.end());
		vec.erase(unique(vec.begin(), vec.end()), vec.end());
	}

public:
	CompressedRange() {}
	CompressedRange(vector<ll> &vec)
	{
		init = vec;
		compress(init);
	}
	int index(ll val)
	{
		return lower_bound(init.begin(), init.end(), val) - init.begin();
	}
	ll initVal(int idx)
	{
		return init[idx];
	}
};

struct WaveletMatrix
{
	int n, maxLog;
	vector<SuccinctBitVector> bv;
	vector<int> mid;
	CompressedRange cr;
	WaveletMatrix() {}
	WaveletMatrix(vector<ll> arr) : cr(arr)
	{
		n = arr.size();
		if (n == 0)
			return;

		vector<int> compArr(n);
		int maxVal = 0;
		for (int i = 0; i < n; i++)
		{
			compArr[i] = cr.index(arr[i]);
			if (compArr[i] > maxVal)
				maxVal = compArr[i];
		}

		maxLog = (maxVal == 0 ? 1 : 32 - __builtin_clz(maxVal));
		bv.assign(maxLog, SuccinctBitVector(n));
		mid.assign(maxLog, 0);
		vector<int> temp(n);

		for (int bit = maxLog - 1; bit >= 0; bit--)
		{
			for (int i = 0; i < n; i++)
				if ((compArr[i] >> bit) & 1)
					bv[bit].setBit(i);

			bv[bit].build();
			mid[bit] = bv[bit].rank0(n);

			auto it0 = temp.begin();
			auto it1 = temp.begin() + mid[bit];

			for (int i = 0; i < n; i++)
			{
				if ((compArr[i] >> bit) & 1)
					*it1++ = compArr[i];
				else
					*it0++ = compArr[i];
			}

			compArr = temp;
		}
	}

	ll kthSmallest(int left, int right, int k)
	{
		if (left > right || k < 1 || k > right - left + 1)
			return -1;

		int l = left;
		int r = right + 1;
		int res = 0;

		for (int bit = maxLog - 1; bit >= 0; bit--)
		{
			int c0 = bv[bit].rank0(r) - bv[bit].rank0(l);
			if (k <= c0)
			{
				l = bv[bit].rank0(l);
				r = bv[bit].rank0(r);
			}
			else
			{
				k -= c0;
				res |= (1 << bit);
				l = mid[bit] + bv[bit].rank1(l);
				r = mid[bit] + bv[bit].rank1(r);
			}
		}
		return cr.initVal(res);
	}

private:
	int countLessMatrix(int left, int right, int k)
	{
		if (k <= 0)
			return 0;
		if (maxLog < 32 && k >= (1 << maxLog))
			return right - left + 1;

		int l = left;
		int r = right + 1;
		int ans = 0;

		for (int bit = maxLog - 1; bit >= 0; bit--)
		{
			int c0 = bv[bit].rank0(r) - bv[bit].rank0(l);
			if ((k >> bit) & 1)
			{
				ans += c0;
				l = mid[bit] + bv[bit].rank1(l);
				r = mid[bit] + bv[bit].rank1(r);
			}
			else
			{
				l = bv[bit].rank0(l);
				r = bv[bit].rank0(r);
			}
		}
		return ans;
	}

	ll query(int l, int r, int req, int bit, int res)
	{
		// If the current sub-range size is <= req, impossible to find a majority
		if (r - l <= req)
			return -1;

		if (bit < 0)
			return cr.initVal(res);

		int c0 = bv[bit].rank0(r) - bv[bit].rank0(l);
		int c1 = (r - l) - c0;

		if (c0 > req)
		{
			int l0 = bv[bit].rank0(l);
			int r0 = bv[bit].rank0(r);
			ll ans = query(l0, r0, req, bit - 1, res);
			if (ans != -1)
				return ans;
		}
		if (c1 > req)
		{
			int l1 = mid[bit] + bv[bit].rank1(l);
			int r1 = mid[bit] + bv[bit].rank1(r);
			ll ans = query(l1, r1, req, bit - 1, res | (1 << bit));
			if (ans != -1)
				return ans;
		}
		return -1;
	}

public:
	int countLess(int left, int right, ll k) { return countLessMatrix(left, right, cr.index(k)); }

	int countLessEq(int left, int right, ll k) { return countLessMatrix(left, right, cr.index(k + 1)); }

	int countK(int left, int right, ll k) { return countLessEq(left, right, k) - countLess(left, right, k); }

	int countGreater(int left, int right, ll k) { return (right - left + 1) - countLessEq(left, right, k); }

	int countGreaterEq(int left, int right, ll k) { return (right - left + 1) - countLess(left, right, k); }

	// Complexity: (length / req) * log(max A)
	int findMoreThan(int l, int r, int req) { return query(l, r + 1, req, maxLog - 1, 0); }
};

struct HeavyLightDecomposition
{
private:
	int timer = 0;
	vector<int> parent, depth, heavy, head, in, out, size;
	WaveletMatrix wt;

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
		wt = WaveletMatrix(baseArray);
	}

	ll getDepth(ll u) const
	{
		return depth[u];
	}

	ll queryPath(int u, int v, int k)
	{
		ll res = 0; // Neutral value for the query
		while (head[u] != head[v])
		{
			if (depth[head[u]] < depth[head[v]])
				swap(u, v);
			res += wt.countGreater(in[head[u]], in[u], k);
			u = parent[head[u]];
		}
		if (depth[u] > depth[v])
			swap(u, v);
		res += wt.countGreater(in[u], in[v], k);
		return res;
	}

	ll querySubtree(int u, int k)
	{
		return wt.countGreater(in[u], out[u], k);
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
	cin >> t;
	while (t--)
	{
		cin >> N >> Q;
		vector<vector<ll>> Tree(N + 1);
		vector<ll> val(N + 1);
		for (int u = 1; u <= N; u++)
			cin >> val[u];
		int anyNode = 1;
		for (int i{}; i < N - 1; i++)
		{
			ll u, v;
			cin >> u >> v;
			anyNode = u;
			Tree[u].push_back(v);
			Tree[v].push_back(u);
		}

		ll root = 1;
		// If the tree is not rooted
		root = anyNode;
		HeavyLightDecomposition hld(Tree, root, val);
		while (Q--)
		{
			int u, v, k;
			cin >> u >> v >> k;
			cout << hld.queryPath(u, v, k) << endl;
		}
	}
	return 0;
}