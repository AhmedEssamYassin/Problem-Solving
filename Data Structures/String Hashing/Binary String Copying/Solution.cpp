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

const ll mod = (1LL << 61) - 1;

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

static int autoCall = (precompute(2e5), 0);

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
	HashRange(const string &t) : p(t.size()), s(t.size())
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

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
	int t = 1;
	ll N, m;
	cin >> t;
	while (t--)
	{
		cin >> N >> m;
		string str;
		cin >> str;
		HashRange hashStr = str;
		HashRange hash1 = string(N, '1');
		HashRange hash0 = string(N, '0');
		vector<ll> pref1(N, 0);
		pref1[0] = (str[0] == '1');
		for (int i{1}; i < N; i++)
			pref1[i] = pref1[i - 1] + (str[i] == '1');

		vector<ll> pref0(N, 0);
		pref0[0] = (str[0] == '0');
		for (int i{1}; i < N; i++)
			pref0[i] = pref0[i - 1] + (str[i] == '0');

		vector<Hash> ans;
		while (m--)
		{
			int L, R;
			cin >> L >> R;
			L--, R--;
			ll ones = pref1[R] - (L ? pref1[L - 1] : 0);
			ll zeros = pref0[R] - (L ? pref0[L - 1] : 0);
			Hash First = hashStr.get(0, L - 1);
			Hash Last = hashStr.get(R + 1, N - 1);
			Hash Middle = hash0.get(0, zeros - 1) + hash1.get(0, ones - 1);
			ans.push_back(First + Middle + Last);
		}
		sort(ans.begin(), ans.end());
		ans.erase(unique(ans.begin(), ans.end()), ans.end());
		cout << ans.size() << endl;
	}
	return 0;
}