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
	CoordinateCompression(vector<ll> &vec)
	{
		init = vec;
		compress(init);
	}
	int index(ll val) { return lower_bound(init.begin(), init.end(), val) - init.begin(); }
	ll initialValue(int idx) { return init[idx]; }
};

struct DynamicSegmentTree
{
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		ll value;
		Node *L, *R; // Pointers to left child and right child

		Node() // Constructor
		{
			value = 0;
			L = nullptr;
			R = nullptr;
		}
		Node(const ll &val)
		{
			value = val;
			L = nullptr;
			R = nullptr;
		}

		~Node() // Destructor. Notice the "~" character before the struct name.
		{
			delete L;
			delete R;
			L = nullptr;
			R = nullptr;
		}
	};
	ll N;
	Node *root;
	void merge(Node *&segNode)
	{
		segNode->value = (segNode->L ? segNode->L->value : 0) + (segNode->R ? segNode->R->value : 0);
	}
	void update(ll left, ll right, Node *&curr, ll idx, const ll &newValue)
	{
		// idx is not in range [left, right]
		if (left > idx || right < idx)
			return;
		if (curr == nullptr)
			curr = new Node();
		// curr is the Node that manage only ith element
		if (left == right)
		{
			curr->value += newValue;
			return;
		}

		update(left, mid, curr->L, idx, newValue);
		update(mid + 1, right, curr->R, idx, newValue);
		merge(curr);
	}
	ll query(ll left, ll right, Node *curr, ll leftQuery, ll rightQuery)
	{
		// [left, right] doesn't intersect with [leftQuery, rightQuery]
		if (curr == nullptr || left > rightQuery || right < leftQuery)
			return 0;

		// [left, curR] is inside [leftQuery, rightQuery]
		if (leftQuery <= left && right <= rightQuery)
			return curr->value;

		ll leftSegment = query(left, mid, curr->L, leftQuery, rightQuery);
		ll rightSegment = query(mid + 1, right, curr->R, leftQuery, rightQuery);
		return (leftSegment + rightSegment);
	}

public:
	DynamicSegmentTree(ll rangeSize = 1e9)
	{
		root = new Node();
		N = rangeSize + 1;
	}
	void update(ll idx, const ll &val)
	{
		update(0, N, root, idx, val);
	}
	ll query(ll left, ll right)
	{
		ll ans = query(0, N, root, left, right);
		return ans;
	}
#undef mid
};

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif
	int t = 1;
	ll N;
	// cin >> t;
	while (t--)
	{
		ll n, q;
		cin >> n >> q;
		vector<ll> arr(n), vc;
		for (ll i{}; i < n; i++)
			cin >> arr[i], vc.push_back(arr[i]);

		DynamicSegmentTree segTree(1e6);

		vector<tuple<int, int, int>> query(q);
		for (auto &[type, L, R] : query)
		{
			char x;
			cin >> x;
			if (x == '!')
			{
				type = 1;
				auto &idx = L;
				auto &val = R;
				cin >> idx >> val;
				idx--;

				vc.push_back(val);
			}
			else
			{
				type = 2;
				cin >> L >> R;
				vc.push_back(L);
				vc.push_back(R);
			}
		}
		CoordinateCompression comp(vc);
		for (int i{}; i < n; i++)
			arr[i] = comp.index(arr[i]), segTree.update(arr[i], 1);

		for (auto &[type, L, R] : query)
		{
			R = comp.index(R);
			if (type == 1)
			{
				auto &idx = L;
				auto &val = R;
				segTree.update(arr[idx], -1);
				segTree.update(val, 1);
				arr[idx] = val;
			}
			else
			{
				L = comp.index(L);
				cout << segTree.query(L, R) << '\n';
			}
		}
		return 0;
	}
}