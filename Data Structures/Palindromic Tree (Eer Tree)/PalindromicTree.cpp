#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

template <int sigma = 26, char mch = 'a'>
struct PalindromicTree
{
private:
    vector<array<int, sigma>> to;
    vector<int> len, qLink, slink, par, cnt;
    deque<char> s;
    deque<int> states = {kRoot}, diffs;
    int sz = 2, active = 0;

    // O(|series|) amortized O(1) — walks up qLink/slink until border matches c
    template <bool back = true>
    int seriesLink(int v, char c)
    {
        while (c != getAt<back>(s, len[v] + 1))
        {
            if (c == getAt<back>(s, len[qLink[v]] + 1))
                v = qLink[v];
            else
                v = slink[v];
        }
        return v;
    }

    // O(1) amortized — returns node for palindrome cXc (X = node u),
    // creating it and computing its series/suffix links if needed
    template <bool back = true>
    int getOrCreate(int u, char c)
    {
        u = seriesLink<back>(u, c);
        if (!to[u][c])
        {
            int p = to[seriesLink<back>(qLink[u], c)][c];
            qLink[sz] = p;
            par[sz] = u;
            len[sz] = len[u] + 2;
            slink[sz] = (len[sz] - len[p] == len[p] - len[qLink[p]]) ? (slink[p]) : (p);
            to[u][c] = sz++;
        }
        return to[u][c];
    }

public:
    static constexpr int kRoot = 0, kImaginary = 1;

    // O(q) — initializes the two base nodes: empty palindrome (root) and imaginary node
    PalindromicTree(size_t q)
    {
        q += 2;
        cnt = len = par = qLink = slink = vector(q, 0);
        to.resize(q);
        qLink[kRoot] = slink[kRoot] = kImaginary;
        len[kImaginary] = -1;
    }

    // O(1) — returns element at index idx from back or front of a deque
    template <bool back = true>
    static int getAt(auto const &d, size_t idx)
    {
        if (idx >= size(d))
            return -1;
        if constexpr (back)
            return prev(end(d))[-idx];
        else
            return begin(d)[idx];
    }

    template <bool back = true>
    static void pushTo(auto &d, auto c)
    {
        if constexpr (back)
            d.push_back(c);
        else
            d.push_front(c);
    }

    template <bool back = true>
    static void popFrom(auto &d)
    {
        if constexpr (back)
            d.pop_back();
        else
            d.pop_front();
    }

    // O(1) amortized — appends (back = true) or prepends (back = false) character ch,
    // updating the palindrome suffix series stack
    template <bool back = true>
    void push(char c)
    {
        c -= mch;
        pushTo<back>(s, c);
        int pre = getAt<back>(states, 0);
        int cur = getOrCreate<back>(pre, c);
        active += !(cnt[cur]++);

        int D = 2 + len[pre] - len[cur];
        while (D + len[pre] <= len[cur])
        {
            popFrom<back>(states);
            if (!empty(states))
            {
                pre = getAt<back>(states, 0);
                D += getAt<back>(diffs, 0);
                popFrom<back>(diffs);
            }
            else
                break;
        }
        if (!empty(states))
            pushTo<back>(diffs, D);
        pushTo<back>(states, cur);
    }

    // O(1) amortized — removes character from back (back = true) or front (back = false),
    // restoring the series stack via qLink and par
    template <bool back = true>
    void pop()
    {
        int last = getAt<back>(states, 0);
        active -= !(--cnt[last]);
        popFrom<back>(states);
        popFrom<back>(s);

        array cands = {pair{qLink[last], len[last] - len[qLink[last]]},
                       pair{par[last], 0}};
        for (auto [st, df] : cands)
        {
            if (empty(states))
            {
                states = {st};
                diffs = {df};
            }
            else
            {
                int D = getAt<back>(diffs, 0) - df;
                int pre = getAt<back>(states, 0);
                if (D + len[st] > len[pre])
                {
                    pushTo<back>(states, st);
                    popFrom<back>(diffs);
                    pushTo<back>(diffs, D);
                    pushTo<back>(diffs, df);
                }
            }
        }
        popFrom<back>(diffs);
    }

    void push(char c, bool back) { back ? push<true>(c) : push<false>(c); }
    void pop(bool back) { back ? pop<true>() : pop<false>(); }

    // O(1) — number of distinct palindromic substrings currently in the string
    int distinct() { return active; }

    // O(1) — length of the longest palindromic suffix (back = true) or prefix (back = false)
    template <bool back = true>
    int maxLen() { return len[getAt<back>(states, 0)]; }
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
        int Q;
        cin >> Q;
        PalindromicTree<26, 'a'> palTree(Q);
        while (Q--)
        {
            int type;
            char c;
            cin >> type;
            if (type < 2)
            {
                cin >> c;
                palTree.push(c, type);
            }
            else
                palTree.pop(type % 2);
            cout << palTree.distinct() << " " << palTree.maxLen<0>() << " " << palTree.maxLen<1>() << endl;
        }
    }
    return 0;
}
