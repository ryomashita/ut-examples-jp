この文書は以下のファイルを和訳したものです。
https://github.com/boost-ext/ut/blob/master/README.md
(HEAD: bf8388f61103571dee3061a4ef23292a320d9dbf)

---

> "If you liked it then you "should have put a"_test on it", Beyonce rule

(訳注: The Beyoncé Ruleについては、[Testing Overview](https://abseil.io/resources/swe-book/html/ch11.html)を参照。)

# UT / μt
| [Motivation](#motivation) | [Quick Start](#quick-start) | [Overview](#overview) | [Tutorial](#tutorial) | [Examples](#examples) | [User Guide](#user-guide) | [FAQ](#faq) | [Benchmarks](#benchmarks) |

<details open><summary>C++ <b>single header/single module, macro-free</b> μ(micro)/Unit Testing Framework</summary>
```cpp
#include <boost/ut.hpp> // import boost.ut;

constexpr auto sum(auto... values) { return (values + ...); }

int main() {
  using namespace boost::ut;

  "sum"_test = [] {
    expect(sum(0) == 0_i);
    expect(sum(1, 2) == 3_i);
    expect(sum(1, 2) > 0_i and 41_i == sum(40, 2));
  };
}
```
```sh
Running "sum"...
  sum.cpp:11:FAILED [(3 > 0 and 41 == 42)]
FAILED

===============================================================================
tests:   1 | 1 failed
asserts: 3 | 2 passed | 1 failed
```
> https://godbolt.org/z/f4jEcv9vo

<a name="motivation"></a>
<details open><summary>Motivation</summary>
<p>

