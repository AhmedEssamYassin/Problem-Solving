#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

function<uint64_t()> random_address = []() -> uint64_t
{
	char *p = new char;
	delete p;
	return uint64_t(p);
};
const uint64_t SEED = chrono::steady_clock::now().time_since_epoch().count() * (random_address() | 1);
std::mt19937 rnd(SEED);
#define rng(l, r) uniform_int_distribution<int64_t>(l, r)(rnd)

/*
Large Primes for hash
1000000007
For all coming primes use __int128_t in mult64()
10000000019
100000000003
1000000000039
10000000000037
100000000000031
1000000000000037
10000000000000061
2305843009213693951 = (1LL << 61) - 1

Larger primes take more time
1e9 + 7 is usually sufficient for most of the hashing problems
*/

const ll mod = 1e9 + 7;

#define double_size_t std::conditional_t<(mod > (1LL << 31)), __int128_t, long long>

inline ll add64(const ll &a, const ll &b)
{
	double_size_t res = double_size_t(a) + b;
	if (res >= mod)
		res -= mod;
	return res;
}

inline ll sub64(const ll &a, const ll &b)
{
	double_size_t res = double_size_t(a) - b;
	if (res < 0)
		res += mod;
	if (res >= mod)
		res -= mod;
	return res;
}

inline ll mult64(const ll &a, const ll &b)
{
	return double_size_t(a) * b % mod;
}

ll modPow(ll N, ll power, ll mod)
{
	ll res{1};
	N %= mod;
	while (power)
	{
		if (power & 1)
			res = mult64(res, N);
		N = mult64(N, N);
		power >>= 1;
	}
	return res;
}
ll b1 = rng(100, 1000000000), b2 = rng(b1 + 10, 1000000000);
ll b1I = modPow(b1, mod - 2, mod), b2I = modPow(b2, mod - 2, mod);
vector<ll> Pb1, Pb2, sumB1, sumB2;
void precompute(ll maxSize)
{
	Pb1 = Pb2 = sumB1 = sumB2 = vector<ll>(maxSize + 1, 1);
	for (int i = 1; i <= maxSize; i++)
	{
		Pb1[i] = mult64(Pb1[i - 1], b1);
		Pb2[i] = mult64(Pb2[i - 1], b2);
		sumB1[i] = add64(sumB1[i - 1], Pb1[i]);
		sumB2[i] = add64(sumB2[i - 1], Pb2[i]);
	}
}

static int autoCall = (precompute(5e5), 0);

class Hash
{
	using pll = pair<ll, ll>;
	ll size{};

public:
	pll code{};

	explicit Hash(pair<ll, ll> x = {}, ll sz = {}) : code(std::move(x)), size(sz) {}

	Hash(const ll &x) : code({x % mod, x % mod}), size(1) {}

	Hash(const string &x) : code(), size(0)
	{
		for (const char &c : x)
			*this = *(this) + c;
	}

	void pop_front(int x)
	{
		code.first = sub64(code.first, mult64(Pb1[--size], x));
		code.second = sub64(code.second, mult64(Pb2[size], x));
	}

	void pop_back(int x)
	{
		code.first = mult64((code.first - x + mod), b1I);
		code.second = mult64((code.second - x + mod), b2I);
		size--;
	}
	void clear()
	{
		code = {}, size = 0;
	}
	Hash operator+(const Hash &o)
	{
		Hash ans;
		ans.code = {add64(mult64(code.first, Pb1[o.size]), o.code.first),
					add64(mult64(code.second, Pb2[o.size]), o.code.second)};
		ans.size = size + o.size;
		return ans;
	}
	friend Hash operator+(const Hash &f, const Hash &o)
	{
		return Hash({add64(mult64(f.code.first, Pb1[o.size]), o.code.first),
					 add64(mult64(f.code.second, Pb2[o.size]), o.code.second)},
					f.size + o.size);
	}
	bool operator<(const Hash &o) const
	{
		if (code == o.code)
			return size < o.size;
		return code < o.code;
	}
	bool operator==(const Hash &o) const
	{
		return size == o.size && code == o.code;
	}
	bool operator!=(const Hash &o) const
	{
		return size != o.size || code != o.code;
	}
};

// Rabin-Karp Algorithm
struct HashRange
{
	vector<Hash> p, s;
	HashRange() {}
	HashRange(const vector<ll> &t) : p(t.size()), s(t.size())
	{
		if (t.empty())
			return;
		p.front() = t.front();
		for (int i = 1; i < t.size(); i++)
			p[i] = p[i - 1] + t[i];
		s.back() = t.back();
		for (int i = int(t.size()) - 2; i >= 0; i--)
			s[i] = s[i + 1] + t[i];
	}
	Hash get(int l, int r) const // 0-based indices
	{
		if (l > r)
			return Hash();
		if (!l)
			return p[r];
		return Hash({sub64(p[r].code.first, mult64(p[l - 1].code.first, Pb1[r - l + 1])),
					 sub64(p[r].code.second, mult64(p[l - 1].code.second, Pb2[r - l + 1]))},
					r - l + 1);
	}
	Hash inv(int l, int r) const // 0-based indices
	{
		if (l > r)
			return Hash();
		if (r + 1 == s.size())
			return s[l];
		return Hash({sub64(s[l].code.first, mult64(s[r + 1].code.first, Pb1[r - l + 1])),
					 sub64(s[l].code.second, mult64(s[r + 1].code.second, Pb2[r - l + 1]))},
					r - l + 1);
	}
	void concatenate(const string &t)
	{
		if (t.empty())
			return;
		bool chk = false;
		if (chk = p.empty())
			p.push_back(t[0]);
		for (int i = 0 + chk; i < t.size(); i++)
			p.push_back(p.back() + t[i]);
	}
	void pop_back()
	{
		if (!p.empty())
			p.pop_back();
	}
};
////////////////////////////////////////////////////////////////////////////////////
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
		string str;
		cin >> str;
		vector<ll> val(N + 1);
		for (int u = 1; u <= N; u++)
			val[u] = str[u - 1] - 'a';
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