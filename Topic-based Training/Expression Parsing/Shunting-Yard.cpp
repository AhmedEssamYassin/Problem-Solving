#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct Tok
{
	char op; // 0 -> number
	ll val;
};

int pr(char c) { return c == 'u' ? 3 : ((c == '*' || c == '/') ? 2 : 1); }
bool rightAssoc(char c) { return c == 'u'; }

// Shunting-Yard: infix -> postfix (RPN). Returns false on malformed input.
bool toRPN(const string &s, vector<Tok> &out)
{
	vector<char> st; // operator stack
	bool prev = 0;   // last token was a number or ')'
	auto flush = [&]()
	{ out.push_back({st.back(), 0}); st.pop_back(); };
	for (int i = 0; i < (int)s.size();)
	{
		char c = s[i];
		if (c == ' ')
		{
			i++;
			continue;
		}
		if (isdigit(c))
		{
			if (prev)
				return 0;
			ll x = 0;
			while (isdigit(s[i]))
				x = x * 10 + s[i++] - '0';
			out.push_back({0, x});
			prev = 1;
			continue;
		}
		i++;
		if (c == '(')
		{
			if (prev)
				return 0;
			st.push_back(c);
		}
		else if (c == ')')
		{
			if (!prev)
				return 0;
			while (!st.empty() && st.back() != '(')
				flush();
			if (st.empty())
				return 0; // unmatched ')'
			st.pop_back();
		}
		else if (c == '+' || c == '-' || c == '*' || c == '/')
		{
			if (!prev)
			{
				if (c != '-')
					return 0;
				c = 'u'; // unary minus
			}
			while (!st.empty() && st.back() != '(' &&
			       (pr(st.back()) > pr(c) || (pr(st.back()) == pr(c) && !rightAssoc(c))))
				flush();
			st.push_back(c);
		}
		else
			return 0;
		prev = (c == ')');
	}
	if (!prev)
		return 0;
	while (!st.empty())
	{
		if (st.back() == '(')
			return 0; // unmatched '('
		flush();
	}
	return 1;
}

// Evaluate RPN. Returns false on division by zero.
bool evalRPN(const vector<Tok> &r, ll &res)
{
	vector<ll> v;
	for (auto &t : r)
	{
		if (!t.op)
		{
			v.push_back(t.val);
			continue;
		}
		if (t.op == 'u')
		{
			v.back() = -v.back();
			continue;
		}
		ll b = v.back();
		v.pop_back();
		ll &a = v.back();
		if (t.op == '+')
			a += b;
		else if (t.op == '-')
			a -= b;
		else if (t.op == '*')
			a *= b;
		else if (b)
			a /= b;
		else
			return 0;
	}
	res = v[0];
	return 1;
}

string show(const vector<Tok> &r)
{
	string s;
	for (auto &t : r)
		s += (t.op ? string(1, t.op) : to_string(t.val)) + " ";
	return s;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif
	vector<string> tests = {
	    "1 + 2 * 3",
	    "(1 + 2) * 3",
	    "10 - 4 - 3",
	    "100 / 5 / 2",
	    "-(2 + 3) * 4",
	    "2 * -3 + 10",
	    "--3",
	    "7 / (3 - 3)",
	    "(1 + 2",
	    "2 3",
	    "4 +",
	};
	for (auto &e : tests)
	{
		vector<Tok> r;
		cout << e << "\n  ";
		if (!toRPN(e, r))
			cout << "invalid expression" << endl;
		else
		{
			ll res;
			cout << "RPN: " << show(r) << "\n  ";
			if (evalRPN(r, res))
				cout << "= " << res << endl;
			else
				cout << "division by zero" << endl;
		}
	}
	return 0;
}