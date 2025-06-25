#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

/*
Predicate Functions (Unary/Binary Predicate)

Predicates are functions that return a boolean value: true or false.
Commonly used with STL algorithms to test each element.
They provide a way to define a condition (logic) in a reusable, modular form.

Types:
------
1. Unary Predicate: Takes 1 argument, returns true/false
2. Binary Predicate: Takes 2 arguments, returns true/false

STL Algorithms that accept predicates:
-------------------------------------
all_of(), any_of(), none_of(), count_if(), find_if(), remove_if(), etc.

Map/Set Special Note:
----------------------
- Predicate functions DO NOT work as comparators for ordering in set/map.
- std::set and std::map require a *strict weak ordering*, not just a condition.
- Comparators for set/map must define consistent ordering.
- ** If a comparator does not obey strict weak ordering, elements may not be inserted at all, or the structure may behave incorrectly.

Strict Weak Ordering:
----------------------
- Comparator must define a consistent and transitive order.
- If comp(a, b) is true, then comp(b, a) must be false.
- If neither comp(a, b) nor comp(b, a), then a and b are considered equivalent.
- Needed to keep internal tree structure valid in set/map.

Functors:
---------
Functors (Function Objects) are classes/structs that overload operator().
They can be used as custom comparators or predicates.
They allow maintaining state or logic as part of the object.

Priority Queue Comparator Logic:
-------------------------------
- ** Comparator logic in `priority_queue` is REVERSED.
- A comparator that returns true for smaller values results in a MAX-HEAP.
  Example: `less<int>` makes largest value on top.
- A comparator that returns true for greater values results in a MIN-HEAP.
  Example: `greater<int>` makes smallest value on top.
*/

// Unary Predicate: Check if a number is even
bool isEven(int x)
{
	return x % 2 == 0;
}

// Binary Predicate: Custom comparison - sorts in descending order
bool customCompare(int a, int b)
{
	return a > b;
}

// Functor as comparator for set/map
struct DescendingOrder
{
	bool operator()(int a, int b) const
	{
		return a > b;
	}
};

// Functor for custom struct
struct CompareByLength
{
	bool operator()(const string &a, const string &b) const
	{
		return a.length() < b.length();
	}
};

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE

	vector<int> nums = {1, 2, 3, 4, 5, 6};

	// Using count_if with a predicate
	int evenCount = count_if(nums.begin(), nums.end(), isEven);
	cout << "Even count: " << evenCount << endl; // Prints 3

	// Using remove_if to filter even numbers
	nums.erase(remove_if(nums.begin(), nums.end(), isEven), nums.end());
	cout << "After removing even numbers: ";
	for (int val : nums)
		cout << val << " ";
	cout << endl;

	// Using sort with a binary predicate
	sort(nums.begin(), nums.end(), customCompare);
	cout << "Sorted in descending: ";
	for (int val : nums)
		cout << val << " ";
	cout << endl;

	// set with functor comparator
	set<int, DescendingOrder> s = {5, 3, 1, 4, 2};
	cout << "Set in descending: ";
	for (int val : s)
		cout << val << " ";
	cout << endl;

	// map with functor comparator
	map<string, int, CompareByLength> nameMap;
	nameMap["John"] = 1;
	nameMap["Alexander"] = 2;
	nameMap["Zoe"] = 3;
	cout << "Map sorted by key length: ";
	for (auto &[key, val] : nameMap)
		cout << "(" << key << ": " << val << ") ";
	cout << endl;

	// priority_queue using custom comparator (descending)
	priority_queue<int, vector<int>, less<int>> maxHeap;
	maxHeap.push(10);
	maxHeap.push(5);
	maxHeap.push(20);
	cout << "Max heap top: " << maxHeap.top() << endl; // 20

	// priority_queue with custom functor (min heap)
	priority_queue<int, vector<int>, greater<int>> minHeap;
	minHeap.push(10);
	minHeap.push(5);
	minHeap.push(20);
	cout << "Min heap top: " << minHeap.top() << endl; // 5

	return 0;
}
