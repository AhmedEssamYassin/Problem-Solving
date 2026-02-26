#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

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

struct WaveletMatrix
{
	int n, maxLog;
	vector<SuccinctBitVector> bv;
	vector<int> mid;
	CoordinateCompression cr;

	WaveletMatrix(const vector<ll> &arr) : cr(arr)
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
		return cr.initialValue(res);
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
			return cr.initialValue(res);

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

struct PersistentSegmentTree
{
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		ll value;
		int L;
		int R;
		Node() { value = LLONG_MAX, L = 0, R = 0; }
		Node(const ll &x)
		{
			value = x;
			L = 0;
			R = 0;
		}
		void merge(const Node &leftNode, const Node &rightNode)
		{
			value = min(leftNode.value, rightNode.value);
		}
	};

	vector<Node> tree;
	int maxN;
	vector<int> versions;

	int build(const vector<ll> &arr, int left, int right)
	{
		int newNode = tree.size();
		tree.push_back(Node());

		if (left == right)
		{
			tree[newNode] = arr[left];
			return newNode;
		}

		tree[newNode].L = build(arr, left, mid);
		tree[newNode].R = build(arr, mid + 1, right);
		tree[newNode].merge(tree[tree[newNode].L], tree[tree[newNode].R]);
		return newNode;
	}

	int insert(int node, int left, int right, int i, ll val)
	{
		Node copy = tree[node];
		int newNode = tree.size();
		tree.push_back(copy);

		if (left == right)
		{
			tree[newNode].value = val;
			return newNode;
		}

		if (i <= mid)
			tree[newNode].L = insert(copy.L, left, mid, i, val);
		else
			tree[newNode].R = insert(copy.R, mid + 1, right, i, val);

		tree[newNode].merge(tree[tree[newNode].L], tree[tree[newNode].R]);
		return newNode;
	}

	Node query(int node, int left, int right, int leftQuery, int rightQuery)
	{
		if (!node || leftQuery > right || rightQuery < left)
			return Node();
		if (leftQuery <= left && right <= rightQuery)
			return tree[node];

		Node leftSegment = query(tree[node].L, left, mid, leftQuery, rightQuery);
		Node rightSegment = query(tree[node].R, mid + 1, right, leftQuery, rightQuery);
		Node res;
		res.merge(leftSegment, rightSegment);
		return res;
	}

public:
	PersistentSegmentTree(int n, int maxInsertions) : maxN(n)
	{
		int capacity = (n + maxInsertions) * (__lg(max(1, n)) + 2) + 2;
		tree.reserve(capacity);
		tree.push_back(Node());
		versions.push_back(0);
	}

	// Constructs tree from base array. Returns new version index.
	int build(const vector<ll> &arr)
	{
		tree.reserve(tree.capacity() + 4 * arr.size());
		int root = build(arr, 0, maxN - 1);
		versions.push_back(root);
		return versions.size() - 1;
	}

	// Inserts `value` at `idx` based on a previous version. Returns new version index.
	// Call to create a new historical state
	int insert(int version, int i, ll x)
	{
		assert(version >= 0 && version < versions.size());
		int newRoot = insert(versions[version], 0, maxN - 1, i, x);
		versions.push_back(newRoot);
		return versions.size() - 1;
	}

	// Updates a version IN-PLACE. Does NOT create a new version.
	// Call when modifying a state without needing to keep the old one.
	void update(int version, int i, ll x)
	{
		assert(version >= 1 && version < versions.size());
		// Overwrite the root of the existing version instead of appending a new one
		versions[version] = insert(versions[version], 0, maxN - 1, i, x);
	}

	// Queries sum/result in [leftQuery, rightQuery] for a specific version.
	// Call to answer standard range queries on historical data.
	ll query(int version, int leftQuery, int rightQuery)
	{
		assert(version >= 0 && version < versions.size());
		return query(versions[version], 0, maxN - 1, leftQuery, rightQuery).value;
	}

	// Duplicates a specific version exactly. Returns new version index.
	// Call when a time-step occurs but no actual data changes.
	int copyAndAppend(int k)
	{
		assert(k >= 0 && k < versions.size());
		versions.push_back(versions[k]);
		return versions.size() - 1;
	}

#undef mid
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
		int m;
		cin >> m;
		vector<ll> vc(m), last(m);
		vector<int> lastPos(1e5 + 1, -1);
		vector<int> prev(m, -1), prevPrev(m, -1), lastBad(m, -1);

		PersistentSegmentTree pst(m, 2 * m);
		for (int i{}; i < m; i++)
		{
			cin >> vc[i];

			if (lastPos[vc[i]] == -1)
				last[i] = -1;
			else
				last[i] = lastPos[vc[i]];

			prev[i] = lastPos[vc[i]];
			prevPrev[i] = (prev[i] != -1) ? prev[prev[i]] : -1;

			if (prev[i] == -1 || prevPrev[i] == -1)
				lastBad[i] = -1;
			else
				lastBad[i] = (i - prev[i] == prev[i] - prevPrev[i]) ? lastBad[prev[i]] : prevPrev[i];

			lastPos[vc[i]] = i;

			int currRoot = pst.copyAndAppend(i);
			if (prev[i] != -1)
				pst.update(currRoot, prev[i], INT_MAX);
			pst.update(currRoot, i, lastBad[i]);
		}
		int q;
		cin >> q;
		WaveletMatrix wt(last);

		while (q--)
		{
			int l, r;
			cin >> l >> r;
			--l, --r;
			ll C = wt.countLess(l, r, l);
			bool AP = (pst.query(r + 1, l, r) < l);
			cout << C + (!AP) << endl;
		}
	}
	return 0;
}