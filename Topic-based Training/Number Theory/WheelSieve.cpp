#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

// Works fast until 1e9
vector<int> wheelSieve(const int n, const int subLim = 17, const int bSize = 1 << 15)
{
    static const int rem[] = {1, 7, 11, 13, 17, 19, 23, 29};
    struct SieveInfo
    {
        int p;
        int pos[8];
        SieveInfo(int val) : p(val) {}
    };

    auto getEst = [](const int x) -> int
    { return x > 60184 ? x / (log(x) - 1.1) : max(1.0, x / (log(x) - 1.11)) + 1; };

    const int sq = sqrt(n);
    vector<bool> isP(sq + 1, true);
    for (int i = 2; i * i <= sq; i++)
    {
        if (isP[i])
            for (int j = i * i; j <= sq; j += i)
                isP[j] = false;
    }

    vector<int> res = {2, 3, 5};
    res.resize(getEst(n + 30));
    int cnt = 3;

    vector<SieveInfo> wheel;
    size_t pivot = 0;
    int prod = 1;

    for (int p = 7; p <= sq; p++)
    {
        if (!isP[p])
            continue;
        if (p <= subLim)
        {
            prod *= p;
            pivot++;
            res[cnt++] = p;
        }
        SieveInfo cur(p);
        for (int t = 0; t < 8; ++t)
        {
            int j = (p <= subLim) ? p : p * p;
            while (j % 30 != rem[t])
                j += p << 1;
            cur.pos[t] = j / 30;
        }
        wheel.push_back(cur);
    }

    vector<unsigned char> pat(prod, 0xFF);
    for (size_t i = 0; i < pivot; ++i)
    {
        auto cur = wheel[i];
        const int p = cur.p;
        for (int t = 0; t < 8; ++t)
        {
            const unsigned char mask = ~(1 << t);
            for (int j = cur.pos[t]; j < prod; j += p)
                pat[j] &= mask;
        }
    }

    const int segSize = (bSize + prod - 1) / prod * prod;
    vector<unsigned char> seg(segSize);
    unsigned char *ptr = seg.data();
    const int blocks = (n + 29) / 30;

    for (int start = 0; start < blocks; start += segSize, ptr -= segSize)
    {
        int end = min(blocks, start + segSize);
        for (int i = start; i < end; i += prod)
            copy(pat.begin(), pat.end(), ptr + i);

        if (start == 0)
            ptr[0] &= 0xFE;

        for (size_t i = pivot; i < wheel.size(); i++)
        {
            auto &cur = wheel[i];
            const int p = cur.p;
            for (int t = 0; t < 8; ++t)
            {
                int j = cur.pos[t];
                const unsigned char mask = ~(1 << t);
                for (; j < end; j += p)
                    ptr[j] &= mask;
                cur.pos[t] = j;
            }
        }

        for (int i = start; i < end; i++)
        {
            for (int mask = ptr[i]; mask > 0; mask &= mask - 1)
                res[cnt++] = i * 30 + rem[__builtin_ctz(mask)];
        }
    }

    while (cnt > 0 && res[cnt - 1] > n)
        cnt--;
    res.resize(cnt);
    return res;
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
    // cin >> t;
    while (t--)
    {
        int l, r;
        cin >> l >> r;
        auto primes = wheelSieve(r);
        int cnt = 0;
        auto it = lower_bound(primes.begin(), primes.end(), l);
        for (; it != primes.end(); ++it)
            cnt += (*it % 4 != 3);
        cout << cnt;
    }
    return 0;
}