# Predicate Functions & Comparators in C++

This document explains predicate functions, strict weak ordering, and custom comparators used with STL containers like `set`, `map`, and `priority_queue`.

---

## 🔍 What is a Predicate?

- A **predicate** is a function that returns `true` or `false`.
- Used to test conditions in STL algorithms like `count_if`, `remove_if`, `find_if`, etc.

### Types:
- **Unary Predicate**: Takes one argument → `bool isEven(int x)`
- **Binary Predicate**: Takes two arguments → `bool compare(int a, int b)`

---

## 🧠 Functors (Function Objects)

- A **functor** is a class/struct that overloads `operator()`.
- Can be passed as custom predicates or comparators.
- Advantage: can maintain state or logic inside the object.

```cpp
struct Descending {
    bool operator()(int a, int b) const 
    {
        return a > b;
    }
};
```

---

## ⚠️ Comparator Rules for `set` / `map`

- `std::set` and `std::map` use **comparators** to maintain sorted order.
- These comparators **must follow Strict Weak Ordering**.

### 🔴 If comparator violates strict weak ordering:
- Some elements may not be inserted.
- Tree structure becomes invalid.
- Behavior is undefined or incorrect.

### ✅ Strict Weak Ordering Rules:
1. If `comp(a, b)` is true, then `comp(b, a)` must be false.
2. If `!comp(a, b)` and `!comp(b, a)` ⇒ a and b are equivalent.
3. Comparator must be consistent and transitive.

---

## 💥 priority_queue Comparator Reversal

> ❗ Comparator logic is REVERSED in `priority_queue`.

- `priority_queue<T, vector<T>, less<T>>` → Max Heap (default)
- `priority_queue<T, vector<T>, greater<T>>` → Min Heap

This is **opposite** to the normal logic.

### Example:

```cpp
priority_queue<int, vector<int>, greater<int>> minHeap;
// smallest element will be on top
```

---

## ✅ Example Use Cases

### STL Algorithm:
```cpp
count_if(v.begin(), v.end(), isEven);
```

### Custom Sorted Set:
```cpp
set<int, Descending> s = {5, 2, 3};
```

### Custom Sorted Map by Key Length:
```cpp
map<string, int, CompareByLength> m;
```

### Custom Min-Heap:
```cpp
priority_queue<int, vector<int>, greater<int>> minHeap;
```

---

## 💡 Summary

| Use Case       | Comparator Required | Strict Weak Ordering | Reversed Logic |
|----------------|---------------------|-----------------------|----------------|
| `set`, `map`   | ✅                  | ✅                    | ❌             |
| `priority_queue` | ✅                | ❌ (not required)     | ✅             |
| `algorithms`   | ✅ Predicate        | ❌                   | ❌             |

---

Keep predicates pure and ordering strict!
