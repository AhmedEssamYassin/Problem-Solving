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
	ll kthOne(ll left, ll right, Node *curr, ll k)
	{
		if (curr == nullptr || curr->value < k)
			return -1;
		if (left == right)
			return left;

		ll sumR = (curr->R ? curr->R->value : 0);
		if (sumR >= k)
			return kthOne(mid + 1, right, curr->R, k);
		else
			return kthOne(left, mid, curr->L, k - sumR);
	}

public:
	DynamicSegmentTree(ll rangeSize = 2e9)
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
	ll kthOne(ll k)
	{
		ll ans = kthOne(0, N, root, k);
		return ans;
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
	ll N, Q;
	// cin >> t;
	while (t--)
	{
		cin >> N >> Q;
		vector<ll> vc(N);
		ll cnt[10]{};
		auto addDigits = [&](ll N)
		{
			while (N)
				cnt[N % 10]++, N /= 10;
		};
		auto removeDigits = [&](ll N)
		{
			while (N)
				cnt[N % 10]--, N /= 10;
		};
		DynamicSegmentTree segTree;
		for (auto &x : vc)
			cin >> x, addDigits(x), segTree.update(x, 1);
		while (Q--)
		{
			char type;
			ll k;
			cin >> type >> k;
			if (type == '+')
			{
				if (segTree.query(k, k))
				{
					removeDigits(k);
					segTree.update(k, -1);
				}
				else
				{
					addDigits(k);
					segTree.update(k, +1);
				}
			}
			else if (type == '-')
			{
				ll pos = segTree.kthOne(k);
				if (~pos)
				{
					removeDigits(pos);
					segTree.update(pos, -1);
				}
			}
			else // '?'
			{
				if (segTree.query(k, k))
				{
					ll ans{};
					while (k)
						ans += cnt[k % 10], k /= 10;
					cout << ans << endl;
				}
				else
					cout << -1 << endl;
			}
		}
	}
	return 0;
}