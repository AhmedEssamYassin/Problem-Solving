#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

using u64 = uint64_t;
u64 S = chrono::steady_clock::now().time_since_epoch().count() ^ (u64) new char;
u64 rnd()
{
	S += 0xa0761d6478bd642f;
	__uint128_t t = (__uint128_t)S * (S ^ 0xe7037ed1a0b428db);
	return t >> 64 ^ t;
}
#define rng(l, r) ((l) + (int64_t)((__uint128_t)rnd() * ((r) - (l) + 1) >> 64))

const u64 mod = (1LL << 61) - 1;
inline u64 add64(u64 a, u64 b)
{
	a += b;
	return a >= mod ? a - mod : a;
}
inline u64 sub64(u64 a, u64 b) { return a >= b ? a - b : a + mod - b; }
inline u64 mult64(u64 a, u64 b)
{
	__uint128_t t = (__uint128_t)a * b;
	u64 r = u64(t & mod) + u64(t >> 61);
	return r >= mod ? r - mod : r;
}
u64 modPow(u64 n, u64 e)
{
	u64 r = 1;
	for (; e; e >>= 1, n = mult64(n, n))
		if (e & 1)
			r = mult64(r, n);
	return r;
}

// Collision prob ~ n^2 / 2^122 per comparison.
struct H2
{
	u64 a, b;
	H2 operator+(H2 o) const { return {add64(a, o.a), add64(b, o.b)}; }
	H2 operator-(H2 o) const { return {sub64(a, o.a), sub64(b, o.b)}; }
	H2 operator*(H2 o) const { return {mult64(a, o.a), mult64(b, o.b)}; }
	H2 operator+(u64 x) const { return {add64(a, x), add64(b, x)}; }
	H2 operator-(u64 x) const { return {sub64(a, x), sub64(b, x)}; }
	H2 operator*(u64 x) const { return {mult64(a, x), mult64(b, x)}; }
	auto operator<=>(const H2 &) const = default;
};
const H2 B = {(u64)rng(1 << 20, mod - 2), (u64)rng(1 << 20, mod - 2)};
const H2 BI = {modPow(B.a, mod - 2), modPow(B.b, mod - 2)};
auto [Pb, sumB] = [](int mx)
{
	vector<H2> p(mx + 1, {1, 1}), s(mx + 1, {1, 1});
	for (int i = 1; i <= mx; i++)
	{
		p[i] = p[i - 1] * B;
		s[i] = s[i - 1] + p[i];
	}
	return pair{move(p), move(s)};
}(5e5);

struct Hash
{
	H2 code = {0, 0};
	int size = 0;
	Hash() = default;
	Hash(H2 c, int s) : code(c), size(s) {}
	Hash(ll x) : size(1)
	{
		u64 xm = ((x % (ll)mod) + mod) % mod;
		code = {xm, xm};
	}
	Hash(string_view s) : size(s.size())
	{
		for (uint8_t c : s)
			code = code * B + c;
	}
	void clear() { code = {0, 0}, size = 0; }

	Hash operator+(const Hash &o) const { return {code * Pb[o.size] + o.code, size + o.size}; }
	auto operator<=>(const Hash &) const = default;
};
// Rabin-Karp
struct HashRange
{
	vector<H2> p, s;
	HashRange() = default;
	HashRange(string_view t) : p(t.size() + 1, {0, 0}), s(t.size() + 1, {0, 0})
	{
		int n = t.size();
		for (int i = 0; i < n; i++)
		{
			p[i + 1] = p[i] * B + (uint8_t)t[i];
			s[n - 1 - i] = s[n - i] * B + (uint8_t)t[n - 1 - i];
		}
	}

	Hash get(int l, int r) const { return l <= r ? Hash{p[r + 1] - p[l] * Pb[r - l + 1], r - l + 1} : Hash{}; }
	Hash inv(int l, int r) const { return l <= r ? Hash{s[l] - s[r + 1] * Pb[r - l + 1], r - l + 1} : Hash{}; }
};
/*
A heavy child of a node is the child with the largest subtree size rooted at the child.
A light child of a node is any child that is not a heavy child.
A heavy edge connects a node to its heavy child.
A light edge connects a node to any of its light children.
A heavy path is the path formed by a collection heavy edges.
A light path is the path formed by a collection light edges.
*/

struct HeavyLightDecomposition
{
private:
	int timer = 0;
	vector<int> parent, depth, heavy, head, in, out, size;
	HashRange hashRange;

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

	void dfsHld(const vector<vector<ll>> &Tree, const string &values, string &baseArray, int u, int h)
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
	HeavyLightDecomposition(const vector<vector<ll>> &Tree, int root, const string &values)
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
		string baseArray(N - 1, ' ');
		dfsHld(Tree, values, baseArray, root, 0);
		hashRange = HashRange(baseArray);
	}

	ll getDepth(ll u) const
	{
		return depth[u];
	}

	void queryPath(int u, int v, vector<tuple<int, int, int>> &allPaths)
	{
		// vector<tuple<int, int, int>> allPaths; // {0 or 1, l, r}: 0 for uPaths and 1 for vPaths
		vector<pair<int, int>> uPaths, vPaths;
		while (head[u] != head[v])
		{
			if (depth[head[u]] > depth[head[v]])
			{
				uPaths.push_back({in[head[u]], in[u]});
				u = parent[head[u]];
			}
			else
			{
				vPaths.push_back({in[head[v]], in[v]});
				v = parent[head[v]];
			}
		}
		if (depth[u] > depth[v])
			uPaths.push_back({in[v], in[u]});
		else
			vPaths.push_back({in[u], in[v]});

		reverse(vPaths.begin(), vPaths.end()); // to go from LCA -> v
		for (const auto &[l, r] : uPaths)
			allPaths.emplace_back(0, l, r);
		for (const auto &[l, r] : vPaths)
			allPaths.emplace_back(1, l, r);
	}

	Hash querySubtree(int u)
	{
		return hashRange.get(in[u], out[u]);
	}

	int LCP(int a, int b, int c, int d)
	{
		vector<tuple<int, int, int>> a_to_b_paths;
		queryPath(a, b, a_to_b_paths);
		vector<tuple<int, int, int>> c_to_d_paths;
		queryPath(c, d, c_to_d_paths);

		auto getHash = [&](const tuple<int, int, int> &range, int len) -> Hash
		{
			const auto &[type, l, r] = range;
			if (len <= 0)
				return Hash();
			if (type == 0) // (u -> LCA) path needs backward hash
			{
				// For backward hash, we need to take the suffix of length 'len'
				int newL = r - len + 1;
				return hashRange.inv(newL, r);
			}
			else // (LCA -> v) path needs forward hash
			{
				// For forward hash, we need to take the prefix of length 'len'
				int newR = l + len - 1;
				return hashRange.get(l, newR);
			}
		};

		int totalLCP = 0;
		int i = 0, j = 0;

		while (i < a_to_b_paths.size() && j < c_to_d_paths.size())
		{
			auto &path1 = a_to_b_paths[i];
			auto &path2 = c_to_d_paths[j];
			auto &[type1, l1, r1] = path1;
			auto &[type2, l2, r2] = path2;

			int len1 = r1 - l1 + 1;
			int len2 = r2 - l2 + 1;
			int minLen = min(len1, len2);

			// Binary search to find the longest common prefix in these segments
			int L = 1, R = minLen, curLCP = 0;
			while (L <= R)
			{
				int mid = ((L + R) >> 1);
				Hash hash1 = getHash(path1, mid);
				Hash hash2 = getHash(path2, mid);

				if (hash1 == hash2)
				{
					curLCP = mid;
					L = mid + 1;
				}
				else
					R = mid - 1;
			}

			totalLCP += curLCP;

			// If curLCP < minLen, we found a mismatch
			if (curLCP < minLen)
				break;

			// Move to the next segment or adjust current segments
			if (len1 == curLCP)
				i++;
			else
			{
				// Update the start position for the next iteration
				if (type1 == 0)
					r1 -= curLCP;
				else
					l1 += curLCP;
			}

			if (len2 == curLCP)
				j++;
			else
			{
				// Update the start position for the next iteration
				if (type2 == 0)
					r2 -= curLCP;
				else
					l2 += curLCP;
			}
		}

		return totalLCP;
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
		string str;
		cin >> str;
		string val = " " + str;
		vector<vector<ll>> Tree(N + 1);
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
			ll a, b, c, d;
			cin >> a >> b >> c >> d;
			cout << hld.LCP(a, b, c, d) << endl;
		}
	}
	return 0;
}