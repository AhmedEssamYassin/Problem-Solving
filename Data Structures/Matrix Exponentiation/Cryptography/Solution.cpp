#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

ll mod;
inline ll add64(const ll &a, const ll &b)
{
	ll res = a + b;
	if (res >= mod)
		res -= mod;
	return res;
}

inline ll sub64(const ll &a, const ll &b)
{
	ll res = a - b;
	if (res < 0)
		res += mod;
	if (res >= mod)
		res -= mod;
	return res;
}

inline ll mult64(const ll &a, const ll &b)
{
	return a * b % mod;
}

struct Matrix2D
{
	ll a[2][2] = {{0, 0}, {0, 0}};
	// ==================== CONSTRUCTORS ====================
	// Empty constructor
	Matrix2D() {}

	// Value constructor
	Matrix2D(ll a00, ll a01, ll a10, ll a11)
	{
		a[0][0] = a00;
		a[0][1] = a01;
		a[1][0] = a10;
		a[1][1] = a11;
	}

	// Deep Copy constructor
	Matrix2D(const Matrix2D &other)
	{
		(*this)[0][0] = other[0][0];
		(*this)[0][1] = other[0][1];
		(*this)[1][0] = other[1][0];
		(*this)[1][1] = other[1][1];
	}

	// ==================== ASSIGNMENT OPERATOR ====================
	Matrix2D &operator=(const Matrix2D &other)
	{
		(*this)[0][0] = other[0][0];
		(*this)[0][1] = other[0][1];
		(*this)[1][0] = other[1][0];
		(*this)[1][1] = other[1][1];
		return *this;
	}

	// ==================== ELEMENT ACCESS ====================
	// Element access using operator[]
	ll *operator[](int row)
	{
		assert(row >= 0 && row < 2);
		return a[row];
	}

	const ll *operator[](int row) const
	{
		assert(row >= 0 && row < 2);
		return a[row];
	}

	// ==================== ARITHMETIC OPERATORS ====================
	// Matrix2D multiplication
	Matrix2D operator*(const Matrix2D &other) const
	{
		Matrix2D product(0, 0, 0, 0);
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				for (int k = 0; k < 2; k++)
					product.a[i][k] = add64(product.a[i][k], mult64(a[i][j], other.a[j][k]));
			}
		}
		return product;
	}

	Matrix2D &operator*=(const Matrix2D &other)
	{
		*this = *this * other;
		return *this;
	}

	// Input stream operator
	friend istream &operator>>(istream &is, Matrix2D &matrix)
	{
		for (int i{}; i < 2; i++)
			for (int j{}; j < 2; j++)
				is >> matrix[i][j];
		return is;
	}
};

struct SegmentTree
{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((left + right) >> 1)
private:
	struct Node
	{
		Matrix2D matrix;
		// Constructors
		Node()
		{
			matrix[0][0] = 1;
			matrix[0][1] = 0;
			matrix[1][0] = 0;
			matrix[1][1] = 1;
		}
		Node(const Matrix2D &other) : matrix(other) {}
	};
	int size;
	vector<Node> seg;
	Node merge(const Node &leftNode, const Node &rightNode)
	{
		Node res;
		res.matrix = (leftNode.matrix * rightNode.matrix);
		return res;
	}
	void build(int left, int right, int node, const vector<Matrix2D> &arr)
	{
		if (left == right)
		{
			if (left < arr.size())
				seg[node] = arr[left];
			return;
		}

		// Building left node
		build(left, mid, L, arr);

		// Building right node
		build(mid + 1, right, R, arr);

		// Returning to parent nodes
		seg[node] = merge(seg[L], seg[R]);
	}
	void update(int left, int right, int node, int idx, const Matrix2D &other)
	{
		if (left == right)
		{
			seg[node].matrix = other;
			return;
		}
		if (idx <= mid)
			update(left, mid, L, idx, other);
		else
			update(mid + 1, right, R, idx, other);

		// Updating while returning to parent nodes
		seg[node] = merge(seg[L], seg[R]);
	}
	Node query(int left, int right, int node, int leftQuery, int rightQuery)
	{
		// Out of range
		if (right < leftQuery || left > rightQuery)
			return Node(); // Identity Matrix
		// The whole range is the answer
		if (left >= leftQuery && right <= rightQuery)
			return seg[node];
		Node leftSegment = query(left, mid, L, leftQuery, rightQuery);
		Node rightSegment = query(mid + 1, right, R, leftQuery, rightQuery);
		return merge(leftSegment, rightSegment);
	}

public:
	SegmentTree(const vector<Matrix2D> &arr)
	{
		size = 1;
		int n = arr.size();
		while (size < n)
			size <<= 1;
		seg = vector<Node>(2 * size, Node());
		build(0, size - 1, 0, arr);
	}
	void update(int idx, const Matrix2D &val)
	{
		update(0, size - 1, 0, idx, val);
	}
	Matrix2D query(int left, int right)
	{
		Node ans = query(0, size - 1, 0, left, right);
		return ans.matrix;
	}

#undef L
#undef R
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
	int N, M, L, R;
	cin >> ::mod >> N >> M;
	vector<Matrix2D> vc(N);
	for (int i{}; i < N; i++)
		cin >> vc[i];
	SegmentTree segTree(vc);
	while (M--)
	{
		cin >> L >> R;
		L--, R--; // To be 0-based
		Matrix2D ans = segTree.query(L, R);
		cout << ans[0][0] << " " << ans[0][1] << endl;
		cout << ans[1][0] << " " << ans[1][1] << endl;
		cout << endl;
	}
	return 0;
}