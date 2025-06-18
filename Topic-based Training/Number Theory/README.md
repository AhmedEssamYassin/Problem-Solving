# Number Theory Algorithms Cheatsheet

## 1️⃣ Factorization (Get All Divisors)

| Constraint on $N$ | Input Size | Algorithm | Complexity |
|-------------------|------------|-----------|------------|
| $N \leq 10^9$ | $1$ to $10^3$ | Trial Division | $O(\sqrt{N})$ |
| Known $k$ prime factors | $k \leq 64$ | Iterative generation | $O(d(N) \log N)$ |
| $N \leq 10^7$ | Full range | Sieve-based precomputation | $O(N \log N)$ |
| $N \leq 10^7$ | Full range | Sieve-based $\sigma_0(N)$ | $O(N \log N)$ |
| $N \leq 10^7$ | Full range | Divisor summation (for $\sigma_1(N)$) | $O(N \log N)$ |

---

## 2️⃣ Prime Factorization

| Constraint on $N$ | Input Size | Algorithm | Complexity |
|-------------------|------------|-----------|------------|
| $N \leq 10^9$ | $1$ to $10^3$ | Trial Division | $O(\sqrt{N})$ |
| $N \leq 10^9$ | $10^4$ to $10^5$ | Sieve + prime traversal | $O(\pi(N))$ |
| $N \leq 10^{12}$ | $1$ or too few | Trial division | $O(\sqrt{N})$ |
| $N \leq 10^{7}$ | $10^5$ to $10^6$ | Sieving and LPF | $O(\log{N})$ |
| $N \leq 10^{20}$ | $1$ to $100$ | Pollard Rho | $O(N^{1/4})$ |
| $N \leq 10^{30}$ | $10$ | Pollard Rho + Montgomery | $O(N^{1/4})$ |
| Factorial $N$, $N \leq 10^6$ | $10$ to $50$ | Sieve + Legendre  | $O(\pi(N) \log(N))$ |

---

## 3️⃣ Prime Checking (Primality Test)

| Constraint on $N$ | Input Size | Algorithm | Complexity |
|-------------------|------------|-----------|------------|
| $N \leq 10^9$ | $1$ to $10^3$ | Trial Division | $O(\sqrt{N})$ |
| $N \leq 10^7$ | $10^5$ to $10^6$ | Sieve of Eratosthenes | $O(N \log{\log{N}})$ for precomputation and $O(1)$ for query  |
| $N \leq 10^{36}$ | $100$ to $500$ | Miller-Rabin (strong bases) | $O(k \log^2{N})$  |

---

## 4️⃣ Count Divisors

| Constraint on $N$ | Input Size | Algorithm | Complexity |
|-------------------|------------|-----------|------------|
| $N \leq 10^6$ | Full range | Sieve-based precomputation for $\sigma_0(N)$ | $O(N \log N)$ |
| $N \leq 10^{18}$ | $1$ or too few | Sieve and LPF + Miller-Rabin | $O(N^{1/3})$ |
| $N \leq 10^{30}$ | $10$ | Pollard Rho + Montgomery | $O(N^{1/4})$ |
| Factorial $N$, $N \leq 10^6$ | $10$ to $50$ | Sieve + Legendre  | $O(\pi(N) \log(N))$ |
| Factorial $N$, $N \leq 10^8$ | $1000$ | Linear Sieve  | $O(N) + \sqrt(N)$ |

---

## 5️⃣ Special Functions

| Constraint on N | Input Size | Algorithm | Complexity |
|-------------------|------------|-----------|------------|
| **Euler’s Totient Function $\varphi(N)$** | | | |
| $N \leq 10^9$ | Single query | Trial Division | $O(\sqrt{N})$ |
| $N \leq 10^6$ | Full range | Sieve of Eratosthenes + Totient formula | $O(N)$ |
| $N \leq 10^{30}$ | Single query | Pollard Rho | $O(N^{1/4})$ |
| **Very large exponentiation: Euler Generalization on Fermat's little theorem** | | | |
| **Euler Totient Function Summation: Counting Co-prime pairs from $1$ to $N$** | | | |
| $N \leq 10^{11}$ | Single query | Dirichlet Convolution | $O(N^{2/3})$ |
| **Divisor Summatory Function $\sum_{i=1}^{N} d(i)$** | | | |
| $N \leq 10^{12}$ | Single query | Dirichlet Hyperbola Method | $O(N^{1/3})$ |
| $N \leq 10^7$ | Full range | Precomputation (naive) | $O(N \log N)$ |
| **Prime Counting $\pi(N)$** | | | |
| $N \leq 10^7$ | Full range | Linear Sieve of Eratosthenes | $O(N)$ |
| $N \leq 10^{12}$ | Single query | Meissel-Lehmer | $O(N^{2/3})$ |
