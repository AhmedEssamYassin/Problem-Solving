#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

class CoordinateCompression
{
private:
	vector<ll> init;
	void compress(vector<ll> &vec)
	{
		sort(vec.begin(), vec.end());
		vec.erase(unique(vec.begin(), vec.end()), vec.end());
	}

public:
	CoordinateCompression() {}
	CoordinateCompression(const vector<ll> &vec)
	{
		init = vec;
		compress(init);
	}
	int index(ll val) { return lower_bound(init.begin(), init.end(), val) - init.begin(); }
	ll initialValue(int idx) { return init[idx]; }
};

struct Query
{
	int l, r, t, id;
	int bL, bR;

	Query(int l, int r, int t, int id, int blockSize)
		: l(l), r(r), t(t), id(id), bL(l / blockSize), bR(r / blockSize) {}

	bool operator<(const Query &o) const
	{
		if (bL != o.bL)
			return bL < o.bL;
		if (bR != o.bR)
			return (bL & 1) ? bR < o.bR : bR > o.bR;
		return ((bL + bR) & 1) ? t < o.t : t > o.t;
	}
};

struct Update
{
	int pos;
	ll oldVal, newVal;
};

template <typename AddFn, typename RemoveFn, typename AnswerFn>
vector<ll> runMo(vector<Query> queries, const vector<Update> &updates,
				 vector<ll> &vc,
				 AddFn add, RemoveFn remove,
				 AnswerFn getAnswer)
{
	sort(queries.begin(), queries.end());

	vector<ll> ans(queries.size());
	int curL = 0, curR = -1, curT = -1;

	auto applyUpdate = [&](const Update &u, int curL, int curR)
	{
		if (curL <= u.pos && u.pos <= curR)
		{
			remove(u.pos);
			vc[u.pos] = u.newVal;
			add(u.pos);
		}
		else
			vc[u.pos] = u.newVal;
	};

	auto rollbackUpdate = [&](const Update &u, int curL, int curR)
	{
		if (curL <= u.pos && u.pos <= curR)
		{
			remove(u.pos);
			vc[u.pos] = u.oldVal;
			add(u.pos);
		}
		else
			vc[u.pos] = u.oldVal;
	};
	for (const auto &[l, r, t, id, bL, bR] : queries)
	{
		while (curL > l)
			add(--curL);
		while (curR < r)
			add(++curR);
		while (curL < l)
			remove(curL++);
		while (curR > r)
			remove(curR--);

		while (curT < t)
			applyUpdate(updates[++curT], curL, curR);
		while (curT > t)
			rollbackUpdate(updates[curT--], curL, curR);
		ans[id] = getAnswer();
	}
	return ans;
}

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
		int n, q;
		cin >> n >> q;
		vector<ll> vc(n + 1), vals;
		for (int i{1}; i <= n; i++)
			cin >> vc[i];
		vals = vc; // In case we will use coordinate compression
		vector<ll> cpy = vc;
		vector<Query> queries;
		vector<Update> updates;
		int blockSize = max(1.0, pow(n, 2.0 / 3.0));
		for (int i = 0; i < q; i++)
		{
			int type;
			cin >> type;
			if (type == 1)
			{
				int l, r;
				cin >> l >> r;
				queries.emplace_back(l, r, (int)updates.size() - 1, queries.size(), blockSize);
			}
			else
			{
				int pos;
				ll x;
				cin >> pos >> x;
				vals.push_back(x);
				updates.push_back({pos, vc[pos], x});
				vc[pos] = x;
			}
		}
		vc = cpy;
		CoordinateCompression comp(vals);
		for (int i{1}; i <= n; i++)
			vc[i] = comp.index(vc[i]);
		for (auto &u : updates)
		{
			u.oldVal = comp.index(u.oldVal);
			u.newVal = comp.index(u.newVal);
		}

		auto add = [&](int idx) {};

		auto remove = [&](int idx) {};

		auto getAnswer = [&]() {};

		vector<ll> ans = runMo(queries, updates, vc, add, remove, getAnswer);
		for (const auto &x : ans)
			cout << x << endl;
	}
	return 0;
}