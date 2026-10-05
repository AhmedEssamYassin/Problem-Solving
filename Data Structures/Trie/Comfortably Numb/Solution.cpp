#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

// Persistent binary trie over numbers in [0, 2^B). roots[v] = trie after the first v inserts.
// Node 0 is the empty trie, its children point to itself, so no null checks.
// Each insert adds B + 1 nodes. O(B) per operation.
template <int B = 30>
struct PersistentBinaryTrie
{
	vector<array<int, 2>> ch{{0, 0}};
	vector<int> cnt{0};
	vector<int> roots{0};

	PersistentBinaryTrie(int n = 0)
	{
		ch.reserve(1 + (size_t)n * (B + 1));
		cnt.reserve(1 + (size_t)n * (B + 1));
		roots.reserve(n + 1);
	}

	int clone(int u)
	{
		array<int, 2> c = ch[u];
		ch.push_back(c);
		cnt.push_back(cnt[u]);
		return ch.size() - 1;
	}

	// a[k] = x, where k = number of inserts before this one
	void insert(ll x)
	{
		int old = roots.back(), cur = clone(old);
		roots.push_back(cur);
		cnt[cur]++;
		for (int i = B - 1; i >= 0; i--)
		{
			int b = x >> i & 1, nxt = clone(ch[old][b]);
			ch[cur][b] = nxt;
			cnt[nxt]++;
			cur = nxt, old = ch[old][b];
		}
	}

	// max(x ^ a[j]) for j in [l..r], 0 <= l <= r < number of inserts
	ll maxXor(ll x, int l, int r) const
	{
		int u = roots[r + 1], v = roots[l];
		ll res = 0;
		for (int i = B - 1; i >= 0; i--)
		{
			int b = x >> i & 1;
			if (cnt[ch[u][b ^ 1]] - cnt[ch[v][b ^ 1]] > 0)
				res |= 1LL << i, b ^= 1;
			u = ch[u][b], v = ch[v][b];
		}
		return res;
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
	cin >> t;
	while (t--)
	{
		int n;
		cin >> n;
		vector<ll> vc(n + 1), pref(n + 1);
		for (int i = 1; i <= n; i++)
		{
			cin >> vc[i];
			pref[i] = pref[i - 1] ^ vc[i];
		}

		PersistentBinaryTrie<30> trie(n + 1);
		for (int i = 0; i <= n; i++)
			trie.insert(pref[i]);

		vector<int> NGE(n + 1, n + 1), PGE(n + 1, 0);
		stack<int> st;

		for (int i = 1; i <= n; i++)
		{
			while (!st.empty() && vc[i] >= vc[st.top()])
			{
				NGE[st.top()] = i;
				st.pop();
			}
			st.push(i);
		}

		while (!st.empty())
			st.pop();

		for (int i = n; i >= 1; i--)
		{
			while (!st.empty() && vc[i] > vc[st.top()])
			{
				PGE[st.top()] = i;
				st.pop();
			}
			st.push(i);
		}

		ll res = 0;
		for (int i = 1; i <= n; i++)
		{
			int l = PGE[i] + 1;
			int r = NGE[i] - 1;
			if (i - l <= r - i)
			{
				for (int j = l; j <= i; j++)
				{
					ll xr = vc[i] ^ pref[j - 1];
					res = max(res, trie.maxXor(xr, i, r));
				}
			}
			else
			{
				for (int k = i; k <= r; k++)
				{
					ll xr = vc[i] ^ pref[k];
					res = max(res, trie.maxXor(xr, l - 1, i - 1));
				}
			}
		}
		cout << res << endl;
	}
	return 0;
}