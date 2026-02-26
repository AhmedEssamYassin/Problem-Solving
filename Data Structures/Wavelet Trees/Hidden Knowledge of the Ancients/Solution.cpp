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
		int n, k, l, r;
		cin >> n >> k >> l >> r;
		vector<ll> vc(n), last(n);
		map<ll, int> lastPos;
		for (int i{}; i < n; i++)
		{
			cin >> vc[i];
			if (lastPos.count(vc[i]) == 0)
				last[i] = -1;
			else
				last[i] = lastPos[vc[i]];
			lastPos[vc[i]] = i;
		}
		WaveletMatrix wt(last);
		ll cnt{};
		for (int R = 0, Lk = 0, Lk_1 = 0; R < n; R++)
		{
			while (wt.countLess(Lk, R, Lk) > k)
				Lk++;
			while (wt.countLess(Lk_1, R, Lk_1) >= k)
				Lk_1++;

			ll lenL = R - r + 1, lenR = R - l + 1;
			ll mn = max<ll>(Lk, lenL), mx = min<ll>(Lk_1 - 1, lenR);
			if (mx >= mn)
				cnt += mx - mn + 1;
		}
		cout << cnt << endl;
	}
	return 0;
}