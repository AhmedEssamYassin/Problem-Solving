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
}(7e5);

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

class SplayTree
{
public:
	ll size = 1;
	ll value;
	ll lazyAddValue = 0;
	Hash fHash;
	Hash bHash;
	bool reversed = false;
	SplayTree *left = nullptr;
	SplayTree *right = nullptr;
	SplayTree *parent = nullptr;

	// Constructor
	SplayTree(const ll &value = 0) : value(value), fHash(value), bHash(value) {}

	// Here we merge the nodes in INORDER behaviour (left -> parent -> right)
	// To merge the backward hash, make (right -> parent -> left)
	void update()
	{
		size = 1;
		fHash = value;
		bHash = value;
		if (left != nullptr)
		{
			size += left->size;
			fHash = left->fHash + fHash;
			bHash = bHash + left->bHash;
		}
		if (right != nullptr)
		{
			size += right->size;
			fHash = fHash + right->fHash;
			bHash = right->bHash + bHash;
		}
	}

	inline void applyReverse()
	{
		reversed ^= 1;
		swap(left, right);
	}
	// For lazy propagation
	inline void applyUpdate(ll x)
	{
		value += x;
		lazyAddValue += x;
	}

	void push()
	{
		if (reversed)
		{
			if (left != nullptr)
				left->applyReverse();
			if (right != nullptr)
				right->applyReverse();
			reversed = false;
		}
		if (lazyAddValue != 0)
		{
			if (left != nullptr)
				left->applyUpdate(lazyAddValue);
			if (right != nullptr)
				right->applyUpdate(lazyAddValue);
			lazyAddValue = 0;
		}
	}
	// Rotates the current node with its parent node
	// For example, if current node is left child:
	//     P          N
	//    / \   =>   / \
	//   N   C      A   P
	//  / \            / \
	// A   B          B   C
	void rotate()
	{
		if (parent == nullptr)
			return;
		SplayTree *parent = this->parent;
		SplayTree *grandparent = parent->parent;

		// Update grandparent's child pointer
		if (grandparent != nullptr)
		{
			if (grandparent->right == parent)
				grandparent->right = this;
			else
				grandparent->left = this;
		}

		// Push down lazy values before rotation
		parent->push();
		this->push();

		// Perform rotation based on whether this node is a left or right child
		if (parent->left == this)
		{ // Left child case
			parent->left = this->right;
			this->right = parent;
			if (parent->left != nullptr)
				parent->left->parent = parent;
		}
		else
		{ // Right child case
			parent->right = this->left;
			this->left = parent;
			if (parent->right != nullptr)
				parent->right->parent = parent;
		}

		// Update parent pointers
		parent->parent = this;
		this->parent = grandparent;

		// Update subtree information
		parent->update();
		this->update();
	}

	// Splays the current node to the root of the tree using a sequence of rotations
	// Uses the standard splay tree rotations:
	// - Zig: Single rotation when parent is root
	// - Zig-zig: Double rotation when both node and parent are left/right children
	// - Zig-zag: Double rotation when node and parent are opposite children
	void splay()
	{
		while (this->parent != nullptr)
		{
			SplayTree *parent = this->parent;
			SplayTree *grandparent = parent->parent;

			if (grandparent == nullptr)
			{
				// Zig case - single rotation
				this->rotate();
			}
			else
			{
				bool isParentRightChild = (grandparent->right == parent);
				bool isNodeRightChild = (parent->right == this);

				if (isParentRightChild == isNodeRightChild)
				{
					// Zig-zig case - rotate parent first, then node
					parent->rotate();
				}
				else
				{
					// Zig-zag case - rotate node twice
					this->rotate();
				}
				this->rotate();
			}
		}

		// Push down any lazy values after splaying
		this->push();
	}

	/**
	 * Traverses the tree according to the provided navigation function and returns the final node
	 * @param tree The root of the tree to search
	 * @param goTo Navigation function that returns:
	 *             1 to go left
	 *             -1 to go right
	 *             0 to stop at current node
	 * @return Pair of (final node reached, last direction taken)
	 */
	static std::pair<SplayTree *, int> find(SplayTree *&tree, const std::function<int(const SplayTree *)> &goTo)
	{
		// Handle empty tree case
		if (tree == nullptr)
			return {tree, -1};

		int direction;
		SplayTree *current = tree;

		// Navigate through the tree based on goTo function
		while (true)
		{
			// Handle lazy propagation before accessing node
			current->push();

			// Get direction from navigation function
			direction = goTo(current);

			// Stop if we've reached target node
			if (direction == 0)
				break;

			// Determine next node based on direction
			SplayTree *nextNode = (direction == 1 ? current->left : current->right);

			// Stop if we can't proceed further
			if (nextNode == nullptr)
				break;

			current = nextNode;
		}

		// Bring found node to root
		current->splay();
		return {current, direction};
	}

	/**
	 * Finds the leftmost node in the tree
	 * @param tree Root of the tree
	 * @return Pointer to the leftmost node
	 */
	static SplayTree *getLeftmost(SplayTree *&tree)
	{
		// Always navigate left until we can't anymore
		return find(tree, [&](const SplayTree *temp)
		            { return 1; })
		    .first;
	}

	/**
	 * Finds the rightmost node in the tree
	 * @param tree Root of the tree
	 * @return Pointer to the rightmost node
	 */
	static SplayTree *getRightmost(SplayTree *&tree)
	{
		// Always navigate right until we can't anymore
		return find(tree, [&](const SplayTree *temp)
		            { return -1; })
		    .first;
	}

	static SplayTree *merge(SplayTree *LHS, SplayTree *RHS)
	{
		if (LHS == nullptr)
			return RHS;
		if (RHS == nullptr)
			return LHS;
		SplayTree *temp = getRightmost(LHS);
		temp->right = RHS;
		RHS->parent = temp;
		temp->update();
		return temp;
	}

	/**
	 * Finds the kth element in the tree (1-based indexing)
	 * @param tree Root of the tree
	 * @param k Position to find (1-based)
	 * @return Pointer to the kth node, or nullptr if k is invalid
	 */
	static SplayTree *findKth(SplayTree *tree, int k)
	{
		// Validate input parameters
		if (!tree || k <= 0 || k > tree->size)
			return nullptr;

		SplayTree *current = tree;

		// Navigate through tree using subtree sizes
		while (current != nullptr)
		{
			// Handle lazy propagation
			current->push();

			// Calculate size of left subtree
			int leftSize = current->left ? current->left->size : 0;

			// Check if current node is kth element
			if (k == leftSize + 1)
				break;

			// Navigate left or right based on k
			if (k <= leftSize)
				current = current->left;
			else
			{
				k -= (leftSize + 1); // Adjust k for right subtree
				current = current->right;
			}
		}

		// Bring found node to root
		current->splay();
		return current;
	}

	// Splits the splay tree into two parts [1, k] and [k + 1, N]
	static std::pair<SplayTree *, SplayTree *> split(SplayTree *tree, int k)
	{
		if (tree == nullptr)
			return {nullptr, nullptr};
		if (k <= 0)
			return {nullptr, tree};
		SplayTree *middle = findKth(tree, k);
		if (middle == nullptr)
			return {nullptr, tree};
		SplayTree *R = middle->right;
		if (R != nullptr)
		{
			middle->right = nullptr;
			R->parent = nullptr;
		}
		middle->update();
		return {middle, R};
	}
	// Erase elements in range [L, R]
	SplayTree *eraseRange(SplayTree *tree, int L, int R)
	{
		if (!tree || L > R)
			return tree;

		// Split into three parts: [1, L - 1], [L, R], [R + 1, end]
		auto [leftPart, middle] = SplayTree::split(tree, L - 1);
		auto [toDelete, rightPart] = SplayTree::split(middle, R - L + 1);

		// Free the memory of deleted nodes
		freeNodes(toDelete);

		// Merge remaining parts
		return SplayTree::merge(leftPart, rightPart);
	}
	// Insert elements at position pos
	SplayTree *insert(SplayTree *tree, int pos, const vector<ll> &values)
	{
		int currentSize = (tree ? tree->size : 0);

		// Clamp position to valid range [1, size + 1]
		// Any position > size + 1 becomes size + 1
		pos = max(1, min(pos, currentSize + 1));

		auto [leftPart, rightPart] = SplayTree::split(tree, pos - 1);

		// Create new nodes for values to insert
		SplayTree *newNodes = nullptr;
		for (ll value : values)
		{
			SplayTree *newNode = new SplayTree(value);
			if (!newNodes)
				newNodes = newNode;
			else
			{
				newNodes = SplayTree::merge(newNodes, newNode);
				if (newNodes)
					newNodes->update(); // Update hash after each merge
			}
		}

		// Merge everything back together
		auto result = leftPart;
		if (newNodes)
		{
			result = SplayTree::merge(result, newNodes);
			if (result)
				result->update();
		}
		if (rightPart)
		{
			result = SplayTree::merge(result, rightPart);
			if (result)
				result->update();
		}
		return result;
	}
	void swapRange(SplayTree *&spTree, ll i, ll j, ll k, ll l)
	{
		// First split into three parts: [1, i - 1], [i, j], rest
		auto [leftPart, middle1] = SplayTree::split(spTree, i - 1);
		auto [firstRange, rightPart1] = SplayTree::split(middle1, j - i + 1);

		// From the right part, split into: [j + 1, k - 1], [k, l], rest
		auto [middle2, rightPart2] = SplayTree::split(rightPart1, k - (j + 1));
		auto [secondRange, lastPart] = SplayTree::split(rightPart2, l - k + 1);

		// Now merge in the new order:
		// [1, i - 1] + [k, l] + [j + 1, k - 1] + [i, j] + rest
		spTree = SplayTree::merge(
		    leftPart,
		    SplayTree::merge(
		        secondRange,
		        SplayTree::merge(
		            middle2,
		            SplayTree::merge(
		                firstRange,
		                lastPart))));
	}
	// Helper function to free memory of deleted nodes
	void freeNodes(SplayTree *treeNode)
	{
		if (!treeNode)
			return;
		treeNode->push(); // Ensure lazy propagation is handled
		freeNodes(treeNode->left);
		freeNodes(treeNode->right);
		if (treeNode)
			delete treeNode;
	}
	static void output(SplayTree *treeNode)
	{
		if (treeNode == nullptr)
			return;
		treeNode->push();
		// Inorder traversal
		output(treeNode->left);
		cout << treeNode->value << " ";
		output(treeNode->right);
	}
};

ll rotationT;
ll increment;

// Template function
template <void Process(SplayTree *x)>
tuple<Hash, Hash> apply(SplayTree *tree, int L, int R)
{
	// Split into three parts: [1, L - 1], [L, R], [R + 1, end]
	auto [leftPart, middle] = SplayTree::split(tree, L - 1);
	auto [seg, rightPart] = SplayTree::split(middle, R - L + 1);
	tuple<Hash, Hash> res;
	if (seg != nullptr)
	{
		res = {seg->fHash, seg->bHash};
		Process(seg);
	}
	tree = SplayTree::merge(leftPart, SplayTree::merge(seg, rightPart));
	if (tree)
		tree->update(); // Ensure hash is updated after merge
	return res;
}

inline void getSubtreeData(SplayTree *x)
{
	return;
}

inline void reverse(SplayTree *x)
{
	x->applyReverse();
}

inline void lazyUpdate(SplayTree *x)
{
	x->applyUpdate(increment);
}

inline void shift(SplayTree *x)
{
	int N = x->size;
	if (rotationT >= N)
		rotationT %= N;
	if (rotationT == 0)
		return; // No shift needed
	// Split the segment into two parts: [1, N - rotationT] and [N - rotationT + 1, N]
	auto [L, R] = SplayTree::split(x, N - rotationT);
	x = SplayTree::merge(R, L);
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
	ll N, Q;
	// cin >> t;
	while (t--)
	{
		cin >> N >> Q;
		string str;
		cin >> str;
		SplayTree *spTree = new SplayTree(str[0] - 'a');
		for (int i{1}; i < N; i++)
			spTree = SplayTree::merge(spTree, new SplayTree(str[i] - 'a'));

		for (int i{}; i < Q; i++)
		{
			ll query, L, R, p;
			char c;
			cin >> query;
			if (query == 1)
			{
				cin >> L >> R;
				spTree = spTree->eraseRange(spTree, L, R);
			}
			else if (query == 2)
			{
				cin >> c >> p;
				spTree->insert(spTree, p, {c - 'a'});
			}
			else if (query == 3)
			{
				cin >> L >> R;

				auto rangeData = apply<getSubtreeData>(spTree, L, R);
				if (get<0>(rangeData) == get<1>(rangeData))
					cout << "yes\n";
				else
					cout << "no\n";
			}
			if (spTree != nullptr)
				spTree->splay(); // Null check before splay
		}
	}
	return 0;
}