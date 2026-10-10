#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

// std::priority_queue with lazy deletion for arbitrary value erasure
template <typename T, typename C = vector<T>, typename Cmp = less<T>>
class LazyPQ : public priority_queue<T, C, Cmp>
{
	using B = priority_queue<T, C, Cmp>;
	map<T, int, Cmp> live, pend;
	size_t pc = 0; // live: copies in queue, pend: erased copies still in heap
	void prune()
	{
		for (; !B::empty(); B::pop())
		{
			auto it = pend.find(B::top());
			if (it == pend.end())
				break;
			if (!--it->second)
				pend.erase(it);
			--pc;
		}
	}

public:
	void push(T x)
	{
		++live[x];
		B::push(move(x));
	}
	template <typename... A>
	void emplace(A &&...a) { push(T(forward<A>(a)...)); }
	bool erase(const T &x)
	{ // false if x not in queue
		auto it = live.find(x);
		if (it == live.end())
			return false;
		if (!--it->second)
			live.erase(it);
		++pend[x];
		++pc;
		return true;
	}
	const T &top()
	{
		prune();
		return B::top();
	}
	void pop()
	{
		prune();
		auto it = live.find(B::top());
		if (!--it->second)
			live.erase(it);
		B::pop();
	}
	bool empty() const { return B::size() == pc; }
	size_t size() const { return B::size() - pc; }
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
	}
	return 0;
}
