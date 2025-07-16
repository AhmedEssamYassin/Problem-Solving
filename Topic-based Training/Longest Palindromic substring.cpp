#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

bool isPalindrome(const string &s)
{
    for (int i = 0, j = s.size() - 1; i < j; i++, j--)
    {
        if (s[i] != s[j])
            return false;
    }
    return true;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
    int t = 1;
    // cin >> t;
    while (t--)
    {
        string S, ans = "";
        cin >> S;
        for (int i = 0; i < S.length(); i++)
        {
            for (int j = i + 1; j < S.length(); j++)
            {
                string temp = S.substr(i, j - i + 1);
                if (isPalindrome)
                {
                    if (temp.size() > ans.size())
                        ans = temp;
                }
            }
        }
        cout << ans;
    }
    return 0;
}