# Number Theory Algorithms Cheatsheet

$T$ is the number of queries. Complexity is per query unless it says precompute.
`u64` means $N < 2^{64} \approx 1.8 \times 10^{19}$. `u128` means $N < 2^{128} \approx 3.4 \times 10^{38}$.

## 1. Divisor Enumeration (Get All Divisors)

| Constraint on $N$ | $T$ | Algorithm | Complexity | File |
|-------------------|-----|-----------|------------|------|
| $N \leq 10^9$ | $1$ to $10^3$ | Trial division, then build divisors from $\prod p^e$ | $O(\sqrt{N} + d(N) \log d(N))$ | `generating_factors_from_PF.cpp` |
| $N < 2^{64}$ | $1$ to $10^4$ | Pollard Rho, then build divisors from $\prod p^e$ | Expected $O(N^{1/4}) + O(d(N))$ | `PollardRho_int64.cpp` |
| $N \leq 10^6$ | Full range | Push $i$ into every multiple of $i$ | $O(N \log N)$ time and memory | `precomputeDivisors.cpp` |

$d(N) \leq 103680$ for $N \leq 10^{18}$.

---

## 2. Prime Factorization

| Constraint on $N$ | $T$ | Algorithm | Complexity | File |
|-------------------|-----|-----------|------------|------|
| $N \leq 10^9$ | $1$ to $10^3$ | Trial division | $O(\sqrt{N})$ | `PrimeFactorization_O(sqrtN).cpp` |
| $N \leq 10^9$ | $1$ to $10^3$ | Trial division, mod 30 wheel | $O(\frac{8}{30}\sqrt{N})$ | `wheelPrimeFactorization.cpp` |
| $N \leq 10^9$ | $10^4$ to $10^5$ | Sieve primes to $10^6$, divide by primes $\leq \sqrt{N}$ | $O(\pi(\sqrt{N}))$, about $3.4 \times 10^3$ | `PrimeFactorization_O(PI(sqrtN)).cpp` |
| $N \leq 10^{12}$ | $1$ to $10^3$ | Same file | $O(\pi(\sqrt{N}))$, about $7.8 \times 10^4$ | `PrimeFactorization_O(PI(sqrtN)).cpp` |
| $N \leq 10^6$ | $10^5$ to $10^6$ | Linear sieve (LPF) | $O(N)$ precompute, $O(\log N)$ | `Factorization_O(LogN).cpp` |
| $N < 2^{64}$ | $1$ to $10^5$ | Pollard Rho (Brent) + Montgomery + Miller-Rabin | Expected $O(N^{1/4})$ per factor | `PollardRho_int64.cpp` |
| $N \leq 10^{38}$ | $1$ to $10$ | Pollard Rho (Brent) + 128-bit Montgomery + BPSW | Expected $O(N^{1/4})$ per factor | `PollardRho_int128.cpp` |
| $N \leq 10^{38}$ | $1$ to $10$ | Pollard Rho, then ECM + BPSW | $\exp\left((\sqrt{2}+o(1))\sqrt{\ln p \ln\ln p}\right)$ per factor $p$ | `ECM.cpp` |
| Factorial $N!$, $N \leq 10^6$ | $10$ to $50$ | Sieve + Legendre | $O(N)$ precompute, $O(\pi(N) \log N)$ | `FactorialFactorization.cpp` |

- `PollardRho_int64.cpp`: Brent cycle, one gcd per 128 steps, 64-bit Montgomery, 7-base Miller-Rabin before each split.
- `PollardRho_int128.cpp`: same loop over `u128`. Fine when the second largest prime factor is small. A semiprime with two factors near $10^{19}$ needs about $10^{9.5}$ steps, so use `ECM.cpp` for that.
- `ECM.cpp`: rho runs for at most $2^{15}$ steps, then ECM on Suyama curves (Montgomery form, stage 2 with BSGS).
  $B_1 = 11000$, $B_2 = 1.9 \times 10^6$. After 30 curves: $5 \times 10^4$, $1.3 \times 10^7$. After 100 curves: $2.5 \times 10^5$, $8.4 \times 10^7$.
  Perfect squares are split with `iSqrt`. Results are cached per $N$.

---

## 3. Prime Checking (Primality Test)

| Constraint on $N$ | $T$ | Algorithm | Complexity | File |
|-------------------|-----|-----------|------------|------|
| $N \leq 10^9$ | $1$ to $10^3$ | Trial division | $O(\sqrt{N})$ | `PrimeChecking_O(sqrtN).cpp` |
| $N \leq 10^9$ | $1$ to $10^3$ | Trial division, $6k \pm 1$ | $O(\frac{1}{3}\sqrt{N})$ | `Wheel_PrimeChecking_O(sqrtN).cpp` |
| $N \leq 10^6$ | $10^5$ to $10^6$ | Sieve of Eratosthenes | $O(N \log\log N)$ precompute, $O(1)$ | `SieveOfEratosthenes.cpp` |
| $N \leq 10^6$ | $10^5$ to $10^6$ | Sieve, outer loop to $\sqrt{N}$, bitset only | $O(N \log\log N)$ precompute, $O(1)$ | `SievingTill_sqrtN.cpp` |
| $N \leq 10^7$ | $10^5$ to $10^6$ | Linear sieve | $O(N)$ precompute, $O(1)$ | `SieveOfEratosthenes_Linear.cpp` |
| $N \leq 10^9$ | All primes $\leq N$ | Segmented sieve, mod 30 wheel | $O(N \log\log N)$ | `WheelSieve.cpp` |
| $[L, R]$, $R \leq 10^{12}$, $R - L \leq 10^6$ | $1$ to $10$ | Segmented sieve, odd only | $O((R - L)\log\log R + \sqrt{R})$ | `SegmentedSieve.cpp` |
| $N < 2^{64}$ | Up to $10^6$ | Miller-Rabin, 7 bases | $O(7 \log N)$ mulmods | `PrimeChecking_Miller_Rabin_Jim Sinclair.cpp` |
| $N \leq 10^{38}$ | $10^2$ to $10^4$ | BPSW (MR base 2 + strong Lucas) | $O(\log^2 N)$ | `BPSW.cpp` |

- 7 bases: $\{2, 325, 9375, 28178, 450775, 9780504, 1795265022\}$. Deterministic for all `u64`.
- `BPSW.cpp` uses shift-add `mult128`, which is $O(\log N)$ per mulmod. That gives $O(\log^2 N)$ per test. BPSW has no pseudoprime below $2^{64}$ and no known one above.
- `WheelSieve.cpp` returns every prime up to $N$: about $5 \times 10^7$ ints (200 MB) at $N = 10^9$.

---

## 4. Count Divisors

| Constraint on $N$ | $T$ | Algorithm | Complexity | File |
|-------------------|-----|-----------|------------|------|
| $N \leq 10^6$ | Full range | Harmonic sieve for $\sigma_0$ | $O(N \log N)$ | `precomputeSigma0.cpp` |
| $N \leq 10^{18}$ | $1$ to $10^3$ | Divide by primes $\leq \sqrt[3]{N}$, then Miller-Rabin on the rest | $O(\pi(\sqrt[3]{N}))$ + MR | `CountDivisors_O(cbrtN).cpp` |
| $N < 2^{64}$ | $1$ to $10^5$ | Pollard Rho | Expected $O(N^{1/4})$ | `PollardRho_int64.cpp` |
| $N \leq 10^{38}$ | $1$ to $10$ | Pollard Rho, then ECM + BPSW | Same as §2 | `ECM.cpp` |
| Factorial $N!$, $N \leq 10^6$ | $10$ to $50$ | Sieve + Legendre | $O(\pi(N) \log N)$ | `FactorialFactorization.cpp` |
| Factorial $N!$, $N \leq 10^8$ | $10^3$ | Linear sieve + prefix $\pi$, group primes $> \sqrt{N}$ by $\lfloor N/p \rfloor$ | $O(N)$ precompute, $O(\pi(\sqrt{N}) \log N + \sqrt{N} \log M)$ | `CountDivisors_Of_Factorial.cpp` |
| Factorial $N!$, $N \leq 10^{11}$ | $1$ to $10$ | Lucy Hedgehog for $\pi$, same grouping | $O(N^{3/4})$ time, $O(\sqrt{N})$ memory | `CountDivisors_Of_Factorial-Lucy.cpp` |

- Cube root trick: after removing primes $\leq \sqrt[3]{N}$, the rest is $1$, $p$, $p^2$ or $pq$. Multiply the count by $1$, $2$, $3$ or $4$.
- $d(N!) = \prod_{p \leq N} (e_p + 1) \bmod M$, with $e_p = \sum_{k \geq 1} \lfloor N/p^k \rfloor$.
- For $p > \sqrt{N}$, $e_p = \lfloor N/p \rfloor$. That takes at most $\sqrt{N}$ values, so each block costs one `modPow`.
- `CountDivisors_Of_Factorial.cpp` keeps `cntPrimes` as `vector<int>` of size $10^8$: 400 MB.

---

## 5. Special Functions

| Constraint on $N$ | $T$ | Algorithm | Complexity | File |
|-------------------|-----|-----------|------------|------|
| **Euler's Totient Function $\varphi(N)$** | | | | |
| $N \leq 10^{12}$ | $1$ to $10^3$ | Trial division | $O(\sqrt{N})$ | `EulerTotientFunction.cpp` |
| $N \leq 10^6$ | Full range | Linear sieve | $O(N)$ | `ETF_Sieving.cpp` |
| $N < 2^{64}$ | $1$ to $10^5$ | Pollard Rho | Expected $O(N^{1/4})$ | `EulerTotientFunction_Fast.cpp` |
| $N \leq 10^{38}$ | $1$ to $10$ | Factor with ECM, then $N \prod (1 - 1/p)$ | Same as §2 | `ECM.cpp` |
| **Inverse Totient: smallest $n$ with $\varphi(n) = N$** | | | | |
| $N \leq 2 \times 10^8$ | $1$ to $10^2$ | Primes $p$ with $(p - 1) \mid N$, then backtracking | $O(\sqrt{N})$ + search | `InverseETF.cpp` |
| **Very large exponentiation: Euler's generalization of Fermat's little theorem** | | | | |
| $m \leq 10^{12}$ | Single | Power tower, $\varphi$ by trial division | $O(\sqrt{m} \log m)$ | `Euler_Generalization_O(sqrtN).cpp` |
| $m < 2^{64}$ | Single | Power tower, $\varphi$ by Pollard Rho | $O(m^{1/4} \log m)$ | `Euler_Generalization_PollardRho.cpp` |
| **Totient Summation $\Phi(N) = \sum_{i=1}^{N} \varphi(i)$** | | | | |
| $N \leq 10^{11}$ | Single | Lucy Hedgehog (prime count and prime sum) + Black's algorithm | $O(N^{3/4} / \log N)$ time, $O(\sqrt{N})$ memory | `ETF_Summation.cpp` |
| **Coprime pairs in an array** | | | | |
| $a_i \leq 2 \times 10^6$ | Single | Möbius inclusion-exclusion over $\gcd$ multiples | $O(n + M \log M)$, $M = \max a_i$ | `Counting_CoprimePairs.cpp` |
| **Divisor Summatory Function $D(N) = \sum_{i=1}^{N} d(i)$** | | | | |
| $N \leq 10^6$ | Full range | Prefix sums over `precomputeSigma0` | $O(N \log N)$ | `precomputeSigma0.cpp` |
| $N \leq 10^{12}$ | $1$ to $10^2$ | Dirichlet hyperbola | $O(\sqrt{N})$ | `DivisorSummatory.cpp` |
| **Sum of Divisors $\sigma_1(N)$** | | | | |
| $N < 2^{64}$ | $1$ to $10^5$ | Pollard Rho, then $\prod \frac{p^{e+1} - 1}{p - 1}$ | Expected $O(N^{1/4})$ | `SumOfDivisors.cpp` |
| Factorization given | $k$ primes | Count, sum and product of divisors $\bmod p$ | $O(k \log p)$ | `PrimeFactorsAnalysis.cpp` |
| **Prime Counting $\pi(N)$** | | | | |
| $N \leq 5 \times 10^6$ | Full range | Linear sieve + prefix counts | $O(N)$ | `PI_Function_CountPrimes.cpp` |
| $N \leq 10^{11}$ | Single | Lucy Hedgehog | $O(N^{3/4} / \log N)$ | `ETF_Summation.cpp` |
| $N \leq 10^{12}$ | Single | Meissel-Lehmer | About $O(N^{2/3})$, sieve to $2 \times 10^7$ | `PI_Function_CountPrimes_Meissel-Lehmer.cpp` |

- Generalized Euler: $a^b \equiv a^{(b \bmod \varphi(m)) + \varphi(m)} \pmod m$ for $b \geq \log_2 m$, even if $\gcd(a, m) > 1$. The code does this with `normalize(x) = x < m ? x : m + x % m`.
- Power towers recurse on $m, \varphi(m), \varphi(\varphi(m)), \dots$, which reaches $1$ in $O(\log m)$ steps.
- Coprime pairs $(i, j)$ in $[1, N]^2$: $2\Phi(N) - 1$.
- Hyperbola: $D(N) = 2\sum_{i=1}^{\lfloor\sqrt{N}\rfloor} \lfloor N/i \rfloor - \lfloor\sqrt{N}\rfloor^2$.
- Meissel-Lehmer memory: `primeCount` 80 MB + `phiMemo` 40 MB.