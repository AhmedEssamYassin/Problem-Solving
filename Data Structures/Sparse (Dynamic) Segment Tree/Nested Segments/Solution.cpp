#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

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
	DynamicSegmentTree(ll rangeSize = 2e9)
	{
		root = new Node();
		N = rangeSize + 2;
	}
	void update(ll idx, const ll &val)
	{
		update(-N, N, root, idx, val);
	}
	ll query(ll left, ll right)
	{
		ll ans = query(-N, N, root, left, right);
		return ans;
	}
#undef mid
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
	ll N;
	// cin >> t;
	while (t--)
	{
		cin >> N;
		vector<tuple<ll, ll, int>> intervals(N);
		vector<ll> Rs;
		DynamicSegmentTree segTree;

		int pos = 0;
		for (auto &[L, R, idx] : intervals)
		{
			cin >> L >> R;
			idx = pos++;
		}

		map<ll, ll> ans;
		sort(intervals.begin(), intervals.end()); // Sorting on basis of `L`
		for (int i = N - 1; i >= 0; i--)
		{
			auto &[L, R, idx] = intervals[i];
			ans[idx] = segTree.query(-2e9, R);
			segTree.update(R, 1);
		}
		for (const auto &[idx, cnt] : ans)
			cout << cnt << endl;
	}
	return 0;
}