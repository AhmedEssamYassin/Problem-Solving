#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct DSU
{
	vector<ll> parent, size; // Representative
	int N;
	// The leader should be the parent of itself
	DSU(int _N)
	{
		N = _N;
		parent.resize(N + 1);
		size.resize(N + 1, 1); // Each component if of size 1 initially
		iota(parent.begin(), parent.end(), 0);
	}
	ll Find(ll node)
	{
		if (parent[node] == node)
			return node;

		return parent[node] = Find(parent[node]); // Path compression
	}
	void Union(ll u, ll v)
	{
		ll rep1 = Find(u);
		ll rep2 = Find(v);
		if (rep1 == rep2)
			return;
		// Small-to-large technique
		if (size[rep1] > size[rep2])
			swap(rep1, rep2); // representative of smaller set comes first
		parent[rep1] = rep2;
		size[rep2] += size[rep1];
	}
	bool isSameComponent(ll u, ll v)
	{
		return (Find(u) == Find(v));
	}
	set<ll> findConnectedComponents()
	{
		set<ll> st;
		// Traverse all vertices
		for (int i = 1; i <= N; i++)
			st.insert(Find(parent[i]));

		return st;
	}
};

vector<ll> Prime;
bitset<100001> isPrime;

void Sieve_Of_Eratosthenes(int N)
{
	isPrime.set();				 // Initially Assuming all numbers to be primes
	isPrime[0] = isPrime[1] = 0; // 0 and 1 are NOT primes
	for (ll i{2}; i * i <= N; i++)
	{
		if (isPrime[i])
		{
			for (ll j = i * i; j <= N; j += i)
				isPrime[j] = 0; // Crossing out all the multiples of prime numbers
		}
	}
	for (int i = 1; i <= N; i++)
		if (isPrime[i])
			Prime.push_back(i);
}

void buildPaths(DSU &dsu, const vector<int> &nums, int m)
{
	// There are O(M/log(M)) primes
	vector<bool> exist(m + 1, 0);
	for (auto &x : nums)
		exist[x] = 1;
	for (const auto &u : Prime)
	{
		for (int v = u * 2; v <= m; v += u)
		{
			if (exist[v] == 1)
				dsu.Union(u, v);
		}
	}
}
static int autoCall = (Sieve_Of_Eratosthenes(100000), 0);
class Solution
{
public:
	bool canTraverseAllPairs(vector<int> &nums)
	{
		if (nums.size() == 1)
			return true;
		int m = *max_element(nums.begin(), nums.end());
		bitset<100001> exist;
		for (int &x : nums)
		{
			if (x == 1)
				return false;
			exist[x] = 1;
		}
		nums.clear();
		for (int i = 2; i <= m; i++)
			if (exist[i])
				nums.push_back(i);
		int n = nums.size();
		DSU dsu(m + 1);
		buildPaths(dsu, nums, m);
		for (int i = 1; i < n; i++)
			if (!dsu.isSameComponent(nums[i], nums[0]))
				return false;
		return true;
	}
};