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
テストはソフトウェア開発において非常に重要な要素ですが、C++にはすぐに使える優れたテスト機能がありません。
このことにより、開発のテストエクスペリエンスが低下したり、テストやカバレッジが不足することが一般的によく発生します。

> テストコードは成果物に含まれる必要があります！

さらに、[テスト駆動開発 (TDD)](https://ja.wikipedia.org/wiki/テスト駆動開発)/[ビヘイビア駆動開発 (BDD)](https://ja.wikipedia.org/wiki/ビヘイビア駆動開発) などの十分に確立されたテスト手法も、同様の理由で実施されないことがよくあります。

次のスニペットは、C++プロジェクトにおけるテストの一般的な例です。
```cpp
int main() {
  // should sum numbers
  {
    assert(3 == sum(1, 2));
  }
}
```

このアプローチには、いくつかの問題があります。
- テストに名前がない。（意図を理解する事が難しくなる）
- テストの自動登録がない。（特定のテストだけを実行する方法がない）
- デバッグしづらい。（アサーションはテストの失敗原因に関する情報を提供しない）
- スケールしない。（パラメタ化、複数のスイート、並列実行などをサポートしていない）
- 統合しづらい。（CI統合に必要な、XML形式などカスタム形式での出力をサポートしていない）
- 誤りを生じやすい。（暗黙のキャスト、浮動小数点のひっかう、文字列のポインタ比較など）
- `TDD/BDD` といった確立された実践例を模倣しづらい。（セクション、宣言式のサポートがない）
- ...

`UT` はこれらの問題を、次のシンプルな手順で得られるシンプルなテストエクスペリエンスによって解決しようとしています。
- [シングルヘッダー（モジュール）](https://github.com/boost-ext/ut/blob/master/include/boost/ut.hpp)を入手する。
- プロジェクトにインクルードする。
- いくつかの概念を学ぶ。（[expect, test, suite](#api)）

これで準備完了です！

分かりました、確かに素晴らしいでしょう。でも、なぜ既存のC++テストフレームワークではなく、 `UT` を使う必要があるのでしょう？
* [Boost.Test](https://github.com/boostorg/test)
* [GoogleTest](https://github.com/google/googletest)
* [Catch](https://github.com/catchorg/Catch2)
* [C++テストフレームワークの一覧 (Wikipedia)](https://en.wikipedia.org/wiki/List_of_unit_testing_frameworks#C++)

いい質問ですね！ 以下に示す機能は、 `UT` が試す価値のあるツールであると示すものになるでしょう。

- まず、単体テストフレームワークの基本的な機能（テストの自動登録、アサーション、スイートなど）をすべてサポートしています。
- プロジェクトに簡単に組み込めます。（[シングルヘッダー（モジュール）](https://github.com/boost-ext/ut/blob/master/include/boost/ut.hpp)を組み込むだけです。）
- マクロフリーです。これによりテストエクスペリエンスは大幅に向上します。（代わりにモダンC++の機能を使用します。マクロは強制ではなくオプトインです。 - [まだマクロを使用できますか？](#macros)）
- 柔軟性があります。（[runner, reporter, printer](#examples) などフレームワークの全ての部分がカスタマイズできます。基本的に、他のほとんどの単体テストフレームワークをUTプリミティブの上に実装することが可能です。）
- 学習曲線は短いです。（[expect, test, suit](#api)など、単純な概念がいくつかあるだけです。）
- C++機能を活用し、複雑なテストもサポートします。（[parameterized](#examples)）
- 類似のフレームワークよりコンパイルと実行が早いため、追加の手間をかけず大規模なプロジェクトにも適しています。（[Benchmarks](#benchmarks)）
- [TDD/BDD](#examples) ワークフローをサポートします。
- [Gherkin](#examples) 仕様をサポートします。
- [Spec](#examples) テストをサポートします。
- ...

興味深いと思いませんか？次はこちらをご覧ください。

* [Tutorial](#tutorial)
* [Examples](#examples)
* [User-Guide](#user-guide)

</p>
</details>

<a name="quick-start"></a>
<details open><summary>Quick Start</summary>
<p>

> https://bit.ly/ut-quick-start (slides)

</p>
</details>

<a name="overview"></a>
<details open><summary>Overview</summary>
<p>

* 依存関係なし（[C++20](#cpp-20)）（Tested Compilers: GCC-9+, Clang-9.0+, Apple Clang-11.0.0+, MSVC-2019+*, Clang-cl-9.0+）
* 単一ヘッダー/モジュール（[boost/ut.hpp](https://github.com/boost-ext/ut/blob/master/include/boost/ut.hpp)）
* マクロフリー ([How does it work?](#how-it-works))
* 簡単に使用可能 ([Minimal API](#api) - `test, suite, operators, literals, [expect]`)
* 高速にコンパイル/実行可能 ([Benchmarks](#benchmarks))
* Features ([Assertions](https://github.com/boost-ext/ut/blob/master/example/expect.cpp), [Suites](https://github.com/boost-ext/ut/blob/master/example/suite.cpp), [Tests](https://github.com/boost-ext/ut/blob/master/example/test.cpp), [Sections](https://github.com/boost-ext/ut/blob/master/example/section.cpp), [Parameterized](https://github.com/boost-ext/ut/blob/master/example/parameterized.cpp), [BDD](https://github.com/boost-ext/ut/blob/master/example/BDD.cpp), [Gherkin](https://github.com/boost-ext/ut/blob/master/example/gherkin.cpp), [Spec](https://github.com/boost-ext/ut/blob/master/example/spec.cpp), [Matchers](https://github.com/boost-ext/ut/blob/master/example/matcher.cpp), [Logging](https://github.com/boost-ext/ut/blob/master/example/log.cpp), [Runners](https://github.com/boost-ext/ut/blob/master/example/cfg/runner.cpp), [Reporters](https://github.com/boost-ext/ut/blob/master/example/cfg/reporter.cpp), [...](https://github.com/boost-ext/ut/blob/master/example))
* 統合が容易 ([ApprovalTests.cpp](https://github.com/approvals/ApprovalTests.cpp/releases/tag/v.7.0.0))

</p>
</details>

<a name="tutorial"></a>
<details open><summary>Tutorial</summary>
<p>

<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;Step 0: Get it...</summary>
<p>

>  [here!](https://github.com/boost-ext/ut/blob/master/include/boost/ut.hpp) から最新のヘッダー/モジュールファイルを入手する。

> インクルード/インポートする
```cpp
// #include <boost/ut.hpp> // single header
// import boost.ut;        // single module (C++20)

int main() { }
```

> コンパイル & 実行
```
$CXX main.cpp && ./a.out
```
```
All tests passed (0 assert in 0 test)
```

> （オプション）インストール
```
cmake -Bbuild -H.
cd build && make         # run tests
cd build && make install # install
```

> （オプション）CMakeプロジェクトへの組み込み
このプロジェクトはCMakeコンフィグ・ターゲットを提供します。
`ut` ターゲットを `find_package` コマンドで読み込むだけで、 `Boost::ut` ターゲットをインポートできます。

このターゲットにリンクすると、単一ヘッダファイルに必要なインクルードディレクトリが追加されます。
実例を以下に示します。
```cmake
find_package(ut REQUIRED)
add_library(my_test my_test.cpp)
target_link_libraries(my_test PRIVATE Boost::ut)
```

> （オプション）[Conan](https://conan.io) の組み込み
[boost-ext-ut](https://conan.io/center/boost-ext-ut) パッケージは、 [Conan Center](https://conan.io/center/) から利用可能です。
`boost-ext-ut/1.1.9` をConanfileに追加するだけで、 `ut` ライブラリを利用できます。

</p>
</details>

<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;Step 1: Expect it...</summary>
<p>

> 最初のアサーションを書いてみましょう！

```cpp
int main() {
  boost::ut::expect(true);
}
```
```
All tests passed (1 asserts in 0 test)
```
> https://godbolt.org/z/vfx-eB

> OK. 次は失敗させてみましょう。
```cpp
int main() {
  boost::ut::expect(1 == 2);
}
```
```
main.cpp:4:FAILED [false]
===============================================================================
tests:   0 | 0 failed
asserts: 1 | 0 passed | 1 failed
```

> https://godbolt.org/z/7qTePx

> `1 == 2` は表示されず、 `false` とだけ表示されることに気づきましたか？
> 表示するようにしましょう。

```cpp
int main() {
  using namespace boost::ut;
  expect(1_i == 2);
}
```

```
main.cpp:4:FAILED [1 == 2]
===============================================================================
tests:   0 | 0 failed
asserts: 1 | 0 passed | 1 failed
```

> https://godbolt.org/z/7MXVzu

> OK. `1 == 2` を期待通り表示させることができました。
> User Defined Literal (UDL) `1_i` が使われていることに注目してください。
> `_i` は integer型のコンパイル時定数値を表します。

* 比較演算子のオーバーライドが可能です。
* 異なる型同士の比較はできません。

詳細は [User-guide](#user-guide)　を参照してください。

> 代わりに、 `terse` notation（完結な表記法）を使用することもできます。
```cpp
int main() {
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;

  1_i == 2; // terse notation
}
```

```
main.cpp:7:FAILED [1 == 2]
===============================================================================
tests:   0 | 0 failed
asserts: 1 | 0 passed | 1 failed
```

> https://godbolt.org/z/s77GSm

> 他にもいくつかの構文が利用可能です。
```cpp
expect(1_i == 2);       // UDL syntax
expect(1 == 2_i);       // UDL syntax
expect(that % 1 == 2);  // Matcher syntax
expect(eq(1, 2));       // eq/neq/gt/ge/lt/le
```
```
main.cpp:6:FAILED [1 == 2]
main.cpp:7:FAILED [1 == 2]
main.cpp:8:FAILED [1 == 2]
main.cpp:9:FAILED [1 == 2]
===============================================================================
tests:   0 | 0 failed
asserts: 4 | 0 passed | 4 failed
```

> https://godbolt.org/z/QbgGtc

> OK. しかしアサーションをfatalにすることはできるでしょうか？
> つまり処理を停止させない限り、プログラムがクラッシュしてしまう状況です。
> 難しいことは何もありません。 fatalであるべき式の後に `>>fatal` を加えるだけです。

```cpp
expect((1 == 2_i) >> fatal); // fatal assertion
expect(1_i == 2);            // not executed
```

```
main.cpp:6:FAILED [1 == 2]
===============================================================================
tests:   1 | 1 failed
asserts: 2 | 0 passed | 2 failed
```

> https://godbolt.org/z/WMe8Y1

> でも、私も式は単純な比較よりも複雑です。
> 問題ありません。 `expect` は論理演算子をサポートしています。
```cpp
expect(42l == 42_l and 1 == 2_i); // compound expression
```

```
main.cpp:5:FAILED [(42 == 42 and 1 == 2)]
===============================================================================
tests:   0 | 0 failed
asserts: 1 | 0 passed | 1 failed
```

> https://godbolt.org/z/aEhX4t

> カスタムメッセージを追加することはできますか？
> もちろんです。 `expect` 呼び出しは streamable です。
```cpp
int main() {
  expect(42l == 42_l and 1 == 2_i) << "additional info";
}
```

```
main.cpp:5:FAILED [(42 == 42 and 1 == 2)] additional info
===============================================================================
tests:   0 | 0 failed
asserts: 1 | 0 passed | 1 failed
```

> 素晴らしい。カスタムメッセージとfatalアサーションを同時に使用できますか？
> はい、もちろんです。

```cpp
expect(1 == 2_i) << "fatal assertion" << fatal;
expect(1_i == 2);
```

```
FAILED
in: main.cpp:6 - test condition:  [1 == 2]

 fatal assertion
===============================================================================
tests:   0 | 2 failed
asserts: 0 | 0 passed | 2 failed
```

> 私は [std::expected](https://cpprefjp.github.io/reference/expected/expected.html) を使っているのですが、 失敗時に `error()` をストリームに加えられますか？
> はい、 `std::expected` の `error()` は値がない場合のみ呼び出すことができるため、遅延評価が必要です。

```cpp
"lazy log"_test = [] {
  std::expected<bool, std::string> e = std::unexpected("lazy evaluated");
  expect(e.has_value()) << [&] { return e.error(); } << fatal;
  expect(e.value() == true);
};

```

```
Running test "lazy log"... FAILED
in: main.cpp:12 - test condition:  [false]

 lazy evaluated
===============================================================================
tests:   1 | 2 failed
asserts: 0 | 0 passed | 2 failed
```
> https://godbolt.org/z/v2PDuU

<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;Step 2: Group it...</summary>
<p>

> アサーションは素晴らしいです。しかし、テストをグループ化する方法はありますか？
> `Test cases` はそのための方法です！これにより同じ機能に対するexpectを１つにまとめることができます。

```cpp
"hello world"_test = [] { };
test("hello world") = [] {}; // alternatively
```

```
All tests passed (0 asserts in 1 tests)
```

> https://godbolt.org/z/Bh-EmY

> `asserts` ではなく `tests` がカウントされていることに注目してください。

> それでは、最初のエンドツーエンドのテストケースを書いてみましょう。
```cpp
int main() {
  "hello world"_test = [] {
    int i = 43;
    expect(42_i == i);
  };
}
```

```
Running "hello world"...
  main.cpp:8:FAILED [42 == 43]
FAILED
===============================================================================
tests:   1 | 1 failed
asserts: 1 | 0 passed | 1 failed
```

> https://godbolt.org/z/Y43mXz

> 👍 これで完了です！

> 私はテストをネストして、setup/tear-downを共有したいと考えています。
> ラムダを使い `tests/sections` を表現すれば、簡単に実現できます。
> 次の例を見てみましょう。

```cpp
int main() {
  "[vector]"_test = [] {
    std::vector<int> v(5);

    expect((5_ul == std::size(v)) >> fatal);

    should("resize bigger") = [v] { // or "resize bigger"_test
      mut(v).resize(10);
      expect(10_ul == std::size(v));
    };

    expect((5_ul == std::size(v)) >> fatal);

    should("resize smaller") = [=]() mutable { // or "resize smaller"_test
      v.resize(0);
      expect(0_ul == std::size(v));
    };
  }
}
```

```
All tests passed (4 asserts in 1 tests)
```

> https://godbolt.org/z/XWAdYt

> 素晴らしい！簡単ですね。しかし、私は `BDD` (Behavior Driven Development; 振る舞い駆動開発) を好んでいます。
> そういったサポートはありますか？
> はい！ `BDD`構文を使った例を見てみましょう。

```cpp
int main() {
  "vector"_test = [] {
    given("I have a vector") = [] {
      std::vector<int> v(5);
      expect((5_ul == std::size(v)) >> fatal);

      when("I resize bigger") = [=] {
        mut(v).resize(10);

        then("The size should increase") = [=] {
          expect(10_ul == std::size(v));
        };
      };
    };
  };
}
```

```
All tests passed (2 asserts in 1 tests)
```

> https://godbolt.org/z/dnvxsE

> 更に、 `feature/scenario` エイリアスも使用できます。

```cpp
int main() {
  feature("vector") = [] {
    scenario("size") = [] {
      given("I have a vector") = [] {
        std::vector<int> v(5);
        expect((5_ul == std::size(v)) >> fatal);

        when("I resize bigger") = [=] {
          mut(v).resize(10);

          then("The size should increase") = [=] {
            expect(10_ul == std::size(v));
          };
        };
      };
    };
  };
}
```

```
All tests passed (2 asserts in 1 tests)
```

> https://godbolt.org/z/T4cWss

> `Gherkin` を使うことはできますか？
> はい、もちろんです。 `Gherkin` 仕様を使って、サンプルコードを書き直してみましょう。

```cpp
int main() {
  bdd::gherkin::steps steps = [](auto& steps) {
    steps.feature("Vector") = [&] {
      steps.scenario("*") = [&] {
        steps.given("I have a vector") = [&] {
          std::vector<int> v(5);
          expect((5_ul == std::size(v)) >> fatal);

          steps.when("I resize bigger") = [&] {
            v.resize(10);
          };

          steps.then("The size should increase") = [&] {
            expect(10_ul == std::size(v));
          };
        };
      };
    };
  };

  "Vector"_test = steps |
    R"(
      Feature: Vector
        Scenario: Resize
          Given I have a vector
           When I resize bigger
           Then The size should increase
    )";
}
```

```
All tests passed (2 asserts in 1 tests)
```

> https://godbolt.org/z/jb1d8P

> これは良いですね！しかし、私は `Spec` (Specification; 仕様) を好んでいます。

```cpp
int main() {
  describe("vector") = [] {
    std::vector<int> v(5);
    expect((5_ul == std::size(v)) >> fatal);

    it("should resize bigger") = [v] {
      mut(v).resize(10);
      expect(10_ul == std::size(v));
    };
  };
}
```

```
All tests passed (2 asserts in 1 tests)
```

> https://godbolt.org/z/6jKKzT

> 素晴らしいです。しかし、DRY (Don't Repeat Yourself) を実現するために、同じテストを異なる引数や型で呼び出すにはどうすればいいでしょうか？
> パラメタ化テストを使ってみましょう。

```cpp
int main() {
  for (auto i : std::vector{1, 2, 3}) {
    test("parameterized " + std::to_string(i)) = [i] { // 3 tests
      expect(that % i > 0); // 3 asserts
    };
  }
}
```

```
All tests passed (3 asserts in 3 tests)
```

> https://godbolt.org/z/Utnd6X

> まさにこれです😮！
> もしくは、便利なテスト構文も用意されています👍。

```cpp
int main() {
  "args"_test = [](const auto& arg) {
    expect(arg > 0_i) << "all values greater than 0";
  } | std::vector{1, 2, 3};
}
```

```
All tests passed (3 asserts in 3 tests)
```

> https://godbolt.org/z/6FHtpq

> 更なる詳細は [Examples](#examples) を参照してください。

</p>
</details>

<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;Step 3: Scale it...</summary>
<p>

> 分かった。でも私もプロジェクトはもっと複雑です。スケールするのでしょうか？
> `Test suites` を使えば可能です。 翻訳単位で `suite` を使用すると、
> 内部で定義された `tests` が自動で登録されます。

```cpp
suite errors = [] {
  "exception"_test = [] {
    expect(throws([] { throw 0; })) << "throws any exception";
  };

  "failure"_test = [] {
    expect(aborts([] { assert(false); }));
  };
};

int main() { }
```

```
All tests passed (2 asserts in 2 tests)
```

> https://godbolt.org/z/_ccGwZ

---

> What's next?
> * [Examples](#examples)
> * [User-Guide](#user-guide)

</p>
</details>

</p>
</details>

<a name="examples"></a>
<details open><summary>Examples</summary>
<p>

<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;Assertions</summary>
<p>


```cpp
// operators
expect(0_i == sum());
expect(2_i != sum(1, 2));
expect(sum(1) >= 0_i);
expect(sum(1) <= 1_i);
```

```cpp
// message
expect(3_i == sum(1, 2)) << "wrong sum";
```

```cpp
// expressions
expect(0_i == sum() and 42_i == sum(40, 2));
expect(0_i == sum() or 1_i == sum()) << "compound";
```

```cpp
// matchers
expect(that % 0 == sum());
expect(that % 42 == sum(40, 2) and that % (1 + 2) == sum(1, 2));
expect(that % 1 != 2 or 2_i > 3);
```

```cpp
// eq/neq/gt/ge/lt/le
expect(eq(42, sum(40, 2)));
expect(neq(1, 2));
expect(eq(sum(1), 1) and neq(sum(1, 2), 2));
expect(eq(1, 1) and that % 1 == 1 and 1_i == 1);
```

```cpp
// floating points
expect(42.1_d == 42.101) << "epsilon=0.1";
expect(42.10_d == 42.101) << "epsilon=0.01";
expect(42.10000001 == 42.1_d) << "epsilon=0.1";
```

```cpp
// constant
constexpr auto compile_time_v = 42;
auto run_time_v = 99;
expect(constant<42_i == compile_time_v> and run_time_v == 99_i);
```

```cpp
// failure
expect(1_i == 2) << "should fail";
expect(sum() == 1_i or 2_i == sum()) << "sum?";
```

```
assertions.cpp:53:FAILED [1 == 2] should fail
assertions.cpp:54:FAILED [(0 == 1 or 2 == 0)] sum?
===============================================================================
tests:   0  | 0 failed
asserts: 20 | 18 passed | 2 failed
```

> https://godbolt.org/z/E1c7G5

</p>
</details>

<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;Tests</summary>
<p>

<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Run/Skip/Tag</summary>
<p>

```cpp
"run UDL"_test = [] {
  expect(42_i == 42);
};

skip / "don't run UDL"_test = [] {
  expect(42_i == 43) << "should not fire!";
};
```

```
All tests passed (1 asserts in 1 tests)
1 tests skipped
```

```cpp
test("run function") = [] {
  expect(42_i == 42);
};

skip / test("don't run function") = [] {
  expect(42_i == 43) << "should not fire!";
};
```

```
All tests passed (1 asserts in 1 tests)
1 tests skipped
```

```cpp
tag("nightly") / tag("slow") /
"performance"_test= [] {
  expect(42_i == 42);
};

tag("slow") /
"run slowly"_test= [] {
  expect(42_i == 43) << "should not fire!";
};
```

```
cfg<override> = {.tag = {"nightly"}};
```

```
All tests passed (1 asserts in 1 tests)
1 tests skipped
```

> https://godbolt.org/z/X3_kG4

</p>
</details>

<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Sections</summary>
<p>

```cpp
"[vector]"_test = [] {
  std::vector<int> v(5);

  expect((5_ul == std::size(v)) >> fatal);

  should("resize bigger") = [=] { // or "resize bigger"_test
    mut(v).resize(10);
    expect(10_ul == std::size(v));
  };

  expect((5_ul == std::size(v)) >> fatal);

  should("resize smaller") = [=]() mutable { // or "resize smaller"_test
    v.resize(0);
    expect(0_ul == std::size(v));
  };
};
```

```
All tests passed (4 asserts in 1 tests)
```

> https://godbolt.org/z/cE91bj

</p>
</details>

<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Behavior Driven Development (BDD)</summary>
<p>

```cpp
"Scenario"_test = [] {
  given("I have...") = [] {
    when("I run...") = [] {
      then("I expect...") = [] { expect(1_i == 1); };
      then("I expect...") = [] { expect(1 == 1_i); };
    };
  };
};
```

```
All tests passed (2 asserts in 1 tests)
```

> https://godbolt.org/z/mNBySr

</p>
</details>

<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Gherkin</summary>
<p>

```cpp
int main() {
  bdd::gherkin::steps steps = [](auto& steps) {
    steps.feature("*") = [&] {
      steps.scenario("*") = [&] {
        steps.given("I have a number {value}") = [&](int value) {
          auto number = value;
          steps.when("I add {value} to it") = [&](int value) {
            number += value;
          };
          steps.then("I expect number to be {value}") = [&](int value) {
            expect(that % number == value);
          };
        };
      };
    };
  };

  "Gherkin"_test = steps |
    R"(
      Feature: Number
        Scenario: Addition
          Given I have a number 40
           When I add 2 to it
           Then I expect number to be 42
    )";
}
```

```
All tests passed (1 asserts in 1 tests)
```

> https://godbolt.org/z/BP3hyt

</p>
</details>

<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Spec</summary>
<p>

```cpp
int main() {
  describe("equality") = [] {
    it("should be equal")     = [] { expect(0_i == 0); };
    it("should not be equal") = [] { expect(1_i != 0); };
  };
}
```

```
All tests passed (2 asserts in 1 tests)
```

> https://godbolt.org/z/BXYJ3a

</p>
</details>

<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Parameterized</summary>
<p>

```cpp
for (auto i : std::vector{1, 2, 3}) {
  test("parameterized " + std::to_string(i)) = [i] {
    expect(that % i > 0);
  };
}

"args"_test =
   [](auto arg) {
      expect(arg >= 1_i);
    }
  | std::vector{1, 2, 3};

"types"_test =
    []<class T> {
      expect(std::is_integral_v<T>) << "all types are integrals";
    }
  | std::tuple<bool, int>{};

"args and types"_test =
    []<class TArg>(TArg arg) {
      expect(std::is_integral_v<TArg> >> fatal);
      expect(42_i == arg or "is true"_b == arg);
      expect(type<TArg> == type<int> or type<TArg> == type<bool>);
    }
  | std::tuple{true, 42};
```

```
All tests passed (14 asserts in 10 tests)
```

> https://godbolt.org/z/4xGGdo

> そしてテストが失敗した特定のタイプを知りたいと思うことがあります。
> その場合は、 `reflection::type_name<T>()` を使用できます。

```cpp
"types with type name"_test =
    []<class T>() {
      expect(std::is_unsigned_v<T>) << reflection::type_name<T>() << "is unsigned";
    }
  | std::tuple<unsigned int, float>{};
```

```
Running "types with type name"...PASSED
Running "types with type name"...
  <source>:10:FAILED [false] float is unsigned
FAILED
```

> https://godbolt.org/z/MEnGnbTY4

</p>
</details>


</p>
</details>

<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;Suites</summary>
<p>

```cpp
namespace ut = boost::ut;

ut::suite errors = [] {
  using namespace ut;

  "throws"_test = [] {
    expect(throws([] { throw 0; }));
  };

  "doesn't throw"_test = [] {
    expect(nothrow([]{}));
  };
};

int main() { }
```

```
All tests passed (2 asserts in 2 tests)
```

> https://godbolt.org/z/CFbTP9

</p>
</details>

<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;Misc</summary>
<p>

<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Logging using streams</summary>
<p>

```cpp
"logging"_test = [] {
  log << "pre";
  expect(42_i == 43) << "message on failure";
  log << "post";
};
```

```
Running "logging"...
pre
  logging.cpp:8:FAILED [42 == 43] message on failure
post
FAILED

===============================================================================

tests:   1 | 1 failed
asserts: 1 | 0 passed | 1 failed
```

> https://godbolt.org/z/26fPSY

</p>
</details>

<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Logging using formatting</summary>
<p>
This requires using C++20 with a standard library with std::format support.

```cpp
"logging"_test = [] {
  log("\npre  {} == {}\n", 42, 43);
  expect(42_i == 43) << "message on failure";
  log("\npost {} == {} -> {}\n", 42, 43, 42 == 43);
};
```

```
Running "logging"...
pre  42 == 43
  logging.cpp:8:FAILED [42 == 43] message on failure
post 42 == 43 -> false
FAILED

===============================================================================

tests:   1 | 1 failed
asserts: 1 | 0 passed | 1 failed
```

> https://godbolt.org/z/26fPSY

</p>
</details>

<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Matchers</summary>
<p>

```cpp
"matchers"_test = [] {
  constexpr auto is_between = [](auto lhs, auto rhs) {
    return [=](auto value) {
      return that % value >= lhs and that % value <= rhs;
    };
  };

  expect(is_between(1, 100)(42));
  expect(not is_between(1, 100)(0));
};
```

```
All tests passed (2 asserts in 1 tests)
```

> https://godbolt.org/z/4qwrCi

</p>
</details>

<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Exceptions/Aborts</summary>
<p>

```cpp
"exceptions/aborts"_test = [] {
  expect(throws<std::runtime_error>([] { throw std::runtime_error{""}; }))
    << "throws runtime_error";
  expect(throws([] { throw 0; })) << "throws any exception";
  expect(nothrow([]{})) << "doesn't throw";
  expect(aborts([] { assert(false); }));
};
```

```
All tests passed (4 asserts in 1 tests)
```

> https://godbolt.org/z/A2EehK

</p>
</details>

</p>
</details>

<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;Config</summary>
<p>

<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Runner</summary>
<p>


```cpp
namespace ut = boost::ut;

namespace cfg {
  class runner {
   public:
    template <class... Ts> auto on(ut::events::test<Ts...> test) { test(); }
    template <class... Ts> auto on(ut::events::skip<Ts...>) {}
    template <class TExpr>
    auto on(ut::events::assertion<TExpr>) -> bool { return true; }
    auto on(ut::events::fatal_assertion) {}
    template <class TMsg> auto on(ut::events::log<TMsg>) {}
  };
} // namespace cfg

template<> auto ut::cfg<ut::override> = cfg::runner{};
```

> https://godbolt.org/z/jdg687

</p>
</details>

<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Reporter</summary>
<p>

```cpp
namespace ut = boost::ut;

namespace cfg {
  class reporter {
   public:
    auto on(ut::events::test_begin) -> void {}
    auto on(ut::events::test_run) -> void {}
    auto on(ut::events::test_skip) -> void {}
    auto on(ut::events::test_end) -> void {}
    template <class TMsg> auto on(ut::events::log<TMsg>) -> void {}
    template <class TExpr>
    auto on(ut::events::assertion_pass<TExpr>) -> void {}
    template <class TExpr>
    auto on(ut::events::assertion_fail<TExpr>) -> void {}
    auto on(ut::events::fatal_assertion) -> void {}
    auto on(ut::events::exception) -> void {}
    auto on(ut::events::summary) -> void {}
  };
}  // namespace cfg

template <>
auto ut::cfg<ut::override> = ut::runner<cfg::reporter>{};
```

> https://godbolt.org/z/gsAPKg

</p>
</details>

<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Printer</summary>
<p>

```cpp
namespace ut = boost::ut;

namespace cfg {
struct printer : ut::printer {
  template <class T>
  auto& operator<<(T&& t) {
    std::cerr << std::forward<T>(t);
    return *this;
  }
};
}  // namespace cfg

template <>
auto ut::cfg<ut::override> = ut::runner<ut::reporter<cfg::printer>>{};

int main() {
  using namespace ut;
  "printer"_test = [] {};
}
```

> https://godbolt.org/z/XCscF9

</p>
</details>

</p>
</details>

</p>
</details>

<a name="api"></a>
<a name="configuration"></a>
<a name="user-guide"></a>
<details open><summary>User Guide</summary>
<p>

<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;API</summary>
<p>

```cpp
export module boost.ut; /// __cpp_modules

namespace boost::inline ext::ut::inline v1_1_9 {
  /**
   * Represents test suite object
   */
  struct suite final {
    /**
     * Creates and executes test suite
     * @example suite _ = [] {};
     * @param suite test suite function
     */
    constexpr explicit(false) suite(auto suite);
  };

  /**
   * Creates a test
   * @example "name"_test = [] {};
   * @return test object to be executed
   */
  constexpr auto operator""_test;

  /**
   * Creates a test
   * @example test("name") = [] {};
   * @return test object to be executed
   */
  constexpr auto test = [](const auto name);

  /**
   * Creates a test
   * @example should("name") = [] {};
   * @return test object to be executed
   */
  constexpr auto should = [](const auto name);

  /**
   * Behaviour Driven Development (BDD) helper functions
   * @param name step name
   * @return test object to be executed
   */
  constexpr auto given = [](const auto name);
  constexpr auto when  = [](const auto name);
  constexpr auto then  = [](const auto name);

  /**
   * Evaluates an expression
   * @example expect(42 == 42_i and 1 != 2_i);
   * @param expr expression to be evaluated
   * @param source location https://en.cppreference.com/w/cpp/utility/source_location
   * @return stream
   */
  constexpr OStream& expect(
    Expression expr,
    const std::source_location& location = std::source_location::current()
  );

  struct {
    /**
     * @example (that % 42 == 42);
     * @param expr expression to be evaluated
     */
    [[nodiscard]] constexpr auto operator%(Expression expr) const;
  } that{};

  inline namespace literals {
    /**
     * User defined literals to represent constant values
     * @example 42_i, 0_uc, 1.23_d
     */
    constexpr auto operator""_i;  /// int
    constexpr auto operator""_s;  /// short
    constexpr auto operator""_c;  /// char
    constexpr auto operator""_l;  /// long
    constexpr auto operator""_ll; /// long long
    constexpr auto operator""_u;  /// unsigned
    constexpr auto operator""_uc; /// unsigned char
    constexpr auto operator""_us; /// unsigned short
    constexpr auto operator""_ul; /// unsigned long
    constexpr auto operator""_f;  /// float
    constexpr auto operator""_d;  /// double
    constexpr auto operator""_ld; /// long double

    /**
     * Represents dynamic values
     * @example _i(42), _f(42.)
     */
    constexpr auto _b(bool);
    constexpr auto _c(char);
    constexpr auto _s(short);
    constexpr auto _i(int);
    constexpr auto _l(long);
    constexpr auto _ll(long long);
    constexpr auto _u(unsigned);
    constexpr auto _uc(unsigned char);
    constexpr auto _us(unsigned short);
    constexpr auto _ul(unsigned long);
    constexpr auto _f(float);
    constexpr auto _d(double);
    constexpr auto _ld(long double);

    /**
     * Logical representation of constant boolean (true) value
     * @example "is set"_b     : true
     *          not "is set"_b : false
     */
    constexpr auto operator ""_b;
  } // namespace literals

  inline namespace operators {
    /**
     * Comparison functions to be used in expressions
     * @example eq(42, 42), neq(1, 2)
     */
    constexpr auto eq(Operator lhs, Operator rhs);  /// ==
    constexpr auto neq(Operator lhs, Operator rhs); /// !=
    constexpr auto gt(Operator lhs, Operator rhs);  /// >
    constexpr auto ge(Operator lhs, Operator rhs);  /// >=
    constexpr auto lt(Operator lhs, Operator rhs);  /// <
    constexpr auto le(Operator lhs, Operator rhs);  /// <=

    /**
     * Overloaded comparison operators to be used in expressions
     * @example (42_i != 0)
     */
    constexpr auto operator==;
    constexpr auto operator!=;
    constexpr auto operator>;
    constexpr auto operator>=;
    constexpr auto operator<;
    constexpr auto operator<=;

    /**
     * Overloaded logic operators to be used in expressions
     * @example (42_i != 0 and 1 == 2_i)
     */
    constexpr auto operator and;
    constexpr auto operator or;
    constexpr auto operator not;

    /**
     * Executes parameterized tests
     * @example "parameterized"_test = [](auto arg) {} | std::tuple{1, 2, 3};
     */
    constexpr auto operator|;

    /**
     * Creates tags
     * @example tag("slow") / tag("nightly") / "perf"_test = []{};
     */
    constexpr auto operator/;

    /**
     * Creates a `fatal_assertion` from an expression
     * @example (42_i == 0) >> fatal
     */
    constexpr auto operator>>;
  } // namespace operators

  /**
   * Creates skippable test object
   * @example skip / "don't run"_test = [] { };
   */
  constexpr auto skip = tag("skip");

  struct {
    /**
     * @example log << "message!";
     * @param msg stringable message
     */
    auto& operator<<(Msg msg);
  } log{};

  /**
   * Makes object mutable
   * @example mut(object)
   * @param t object to be mutated
   */
  template<class T> auto mut(const T& t) -> T&;

  /**
   * Default execution flow policy
   */
  class runner {
   public:
    /**
     * @example cfg<override> = {
        .filter  = "test.section.*",
        .colors  = { .none = "" },
        .dry__run = true
       };
     * @param options.filter {default: "*"} runs all tests which names
                                            matches test.section.* filter
     * @param options.colors {default: {
                               .none = "\033[0m",
                               .pass = "\033[32m",
                               .fail  = "\033[31m"
              } if specified then overrides default color values
     * @param options.dry_run {default: false} if true then print test names to be
                                               executed without running them
     */
    auto operator=(options);

    /**
     * @example suite _ = [] {};
     * @param suite() executes suite
     */
    template<class TSuite>
    auto on(ut::events::suite<TSuite>);

    /**
     * @example "name"_test = [] {};
     * @param test.type ["test", "given", "when", "then"]
     * @param test.name "name"
     * @param test.arg parameterized argument
     * @param test() executes test
     */
    template<class... Ts>
    auto on(ut::events::test<Ts...>);

    /**
     * @example skip / "don't run"_test = []{};
     * @param skip.type ["test", "given", "when", "then"]
     * @param skip.name "don't run"
     * @param skip.arg parameterized argument
     */
    template<class... Ts>
    auto on(ut::events::skip<Ts...>);

    /**
     * @example file.cpp:42: expect(42_i == 42);
     * @param assertion.expr 42_i == 42
     * @param assertion.location { "file.cpp", 42 }
     * @return true if expr passes, false otherwise
     */
    template <class TExpr>
    auto on(ut::events::assertion<TExpr>) -> bool;

    /**
     * @example expect((2_i == 1) >> fatal)
     * @note triggered by `fatal`
     *       should std::exit
     */
    auto on(ut::events::fatal_assertion);

    /**
     * @example log << "message"
     * @param log.msg "message"
     */
    template<class TMsg>
    auto on(ut::events::log<TMsg>);

    /**
     * Explicitly runs registered test suites
     * If not called directly test suites are executed with run's destructor
     * @example return run({.report_errors = true})
     * @param run_cfg.report_errors {default: false} if true it prints the summary after runnig
     */
    auto run(run_cfg);

    /**
     * Runs registered test suites if they haven't been explicilty executed already
     */
    ~run();
  };

  /**
   * Default reporter policy
   */
  class reporter {
   public:
    /**
     * @example file.cpp:42: "name"_test = [] {};
     * @param test_begin.type ["test", "given", "when", "then"]
     * @param test_begin.name "name"
     * @param test_begin.location { "file.cpp", 42 }
     */
    auto on(ut::events::test_begin) -> void;

    /**
     * @example "name"_test = [] {};
     * @param test_run.type ["test", "given", "when", "then"]
     * @param test_run.name "name"
     */
    auto on(ut::events::test_run) -> void;

    /**
     * @example "name"_test = [] {};
     * @param test_skip.type ["test", "given", "when", "then"]
     * @param test_skip.name "name"
     */
    auto on(ut::events::test_skip) -> void;

    /**
     * @example "name"_test = [] {};
     * @param test_end.type ["test", "given", "when", "then"]
     * @param test_end.name "name"
     */
    auto on(ut::events::test_end) -> void;

    /**
     * @example log << "message"
     * @param log.msg "message"
     */
    template<class TMsg>
    auto on(ut::events::log<TMsg>) -> void;

    /**
     * @example file.cpp:42: expect(42_i == 42);
     * @param assertion_pass.expr 42_i == 42
     * @param assertion_pass.location { "file.cpp", 42 }
     */
    template <class TExpr>
    auto on(ut::events::assertion_pass<TExpr>) -> void;

    /**
     * @example file.cpp:42: expect(42_i != 42);
     * @param assertion_fail.expr 42_i != 42
     * @param assertion_fail.location { "file.cpp", 42 }
     */
    template <class TExpr>
    auto on(ut::events::assertion_fail<TExpr>) -> void;

    /**
     * @example expect((2_i == 1) >> fatal)
     * @note triggered by `fatal`
     *       should std::exit
     */
    auto on(ut::events::fatal_assertion) -> void;

    /**
     * @example "exception"_test = [] { throw std::runtime_error{""}; };
     */
    auto on(ut::events::exception) -> void;

    /**
     * @note triggered on destruction of runner
     */
    auto on(ut::events::summary) -> void;
  };

  /**
   * Used to override default running policy
   * @example template <> auto cfg<override> = runner<reporter>{};
   */
  struct override {};

  /**
   * Default UT execution policy
   * Can be overwritten with override
   */
  template <class = override> auto cfg = runner<reporter>{};
}
```

</p>
</details>

<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;Configuration</summary>
<p>

| Option | Description | Example |
|-|-|-|
| `BOOST_UT_VERSION`        | Current version | `1'1'9` |

</p>
</details>

</p>
</details>

<a name="faq"></a>
<details open><summary>FAQ</summary>
<p>

<a name="how-it-works"></a>
<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;How does it work?</summary>
<p>

> `suite`

  ```cpp
  /**
   * Reperesents suite object
   * @example suite _ = []{};
   */
  struct suite final {
    /**
     * Assigns and executes test suite
     */
    [[nodiscard]] constexpr explicit(false) suite(Suite suite) {
      suite();
    }
  };
  ```

> `test`

  ```cpp
  /**
   * Creates named test object
   * @example "hello world"_test
   * @return test object
   */
  [[nodiscard]] constexpr Test operator ""_test(const char* name, std::size_t size) {
    return test{{name, size}};
  }
  ```

  ```cpp
  /**
   * Represents test object
   */
  struct test final {
    std::string_view name{}; /// test case name

    /**
     * Assigns and executes test function
     * @param test function
     */
    constexpr auto operator=(const Test& test) {
      std::cout << "Running... " << name << '\n';
      test();
    }
  };
  ```

> `expect`

  ```cpp
  /**
   * Evaluates an expression
   * @example expect(42_i == 42);
   * @param expr expression to be evaluated
   * @param source location https://en.cppreference.com/w/cpp/utility/source_location
   * @return stream
   */
  constexpr OStream& expect(
    Expression expr,
    const std::source_location& location = std::source_location::current()
  ) {
    if (not static_cast<bool>(expr) {
      std::cerr << location.file()
                << ':'
                << location.line()
                << ":FAILED: "
                << expr
                << '\n';
    }

    return std::cerr;
  }
  ```

  ```cpp
  /**
   * Creates constant object for which operators can be overloaded
   * @example 42_i
   * @return integral constant object
   */
  template <char... Cs>
  [[nodiscard]] constexpr Operator operator""_i() -> integral_constant<int, value<Cs...>>;
  ```

  ```cpp
  /**
   * Overloads comparison if at least one of {lhs, rhs} is an Operator
   * @example (42_i == 42)
   * @param lhs Left-hand side operator
   * @param rhs Right-hand side operator
   * @return Comparison object
   */
  [[nodiscard]] constexpr auto operator==(Operator lhs, Operator rhs) {
    return eq{lhs, rhs};
  }
  ```

  ```cpp
  /**
   * Comparison Operator
   */
  template <Operator TLhs, Opeartor TRhs>
  struct eq final {
    TLhs lhs{}; // Left-hand side operator
    TRhs rhs{}; // Right-hand side operator

    /**
     * Performs comparison operatation
     * @return true if expression is succesful
     */
    [[nodiscard]] constexpr explicit operator bool() const {
      return lhs == rhs;
    }

    /**
     * Nicely prints the operation
     */
    friend auto operator<<(OStream& os, const eq& op) -> Ostream& {
      return (os << op.lhs << " == " << op.rhs);
    }
  };
  ```

> `Sections`

  ```cpp
  /**
   * Convenient aliases for creating test named object
   * @example should("return true") = [] {};
   */
  constexpr auto should = [](const auto name) { return test{name}; };
  ```

> `Behaviour Driven Development (BDD)`

  ```cpp
  /**
   * Convenient aliases for creating BDD tests
   * @example feature("Feature") = [] {};
   * @example scenario("Scenario") = [] {};
   * @example given("I have an object") = [] {};
   * @example when("I call it") = [] {};
   * @example then("I should get") = [] {};
   */
  constexpr auto feature  = [](const auto name) { return test{name}; };
  constexpr auto scenario = [](const auto name) { return test{name}; };
  constexpr auto given    = [](const auto name) { return test{name}; };
  constexpr auto when     = [](const auto name) { return test{name}; };
  constexpr auto then     = [](const auto name) { return test{name}; };
  ```

> https://godbolt.org/z/6Nk5Mi

> `Spec`

  ```cpp
  /**
   * Convenient aliases for creating Spec tests
   * @example describe("test") = [] {};
   * @example it("should...") = [] {};
   */
  constexpr auto describe = [](const auto name) { return test{name}; };
  constexpr auto it       = [](const auto name) { return test{name}; };
  ```

> [Example implementation](https://github.com/boost-ext/ut/tree/gh-pages/denver-cpp-2020/example)

> Try it online

* Header - https://godbolt.org/z/x96n8b
* Module - https://wandbox.org/permlink/LrV7WwIgghTP1nrs

</p>
</details>

<a name="fast-compilation-times"></a>
<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;Fast compilation times <a href="#benchmarks">(Benchmarks)</a>?</summary>
<p>

> Implementation

* Leveraging [C++20](#cpp-20) features

* Avoiding unique types for lambda expressions

```cpp
  template <class Test>
    requires not std::convertible_to<Test, void (*)()>>
  constexpr auto operator=(Test test);

vs

  // Compiles 5x faster because it doesn't introduce a new type for each lambda
  constexpr auto operator=(void (*test)());
```

* `Type-name` erasure (allows types/function memoization)

```cpp
  eq<integral_constant<42>, int>{ {}, 42 }

vs

  // Can be memoized - faster to compile
  eq<int, int>{42, 42}
```

* Limiting preprocessor work
  * Single header/module
  * Minimal number of include files

* Simplified versions of
  * `std::function`
  * `std::string_view`

</p>
</details>

<a name="cpp-20"></a>
<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;C++20 features?</summary>
<p>

* API

  * [Source Location](https://eel.is/c++draft/support.srcloc#source.location.syn)
    * Assertions - `expect(false)` - ` __FILE__:__LINE__:FAILED [false]`

  * [Designated initializers](https://eel.is/c++draft/dcl.init#nt:designated-initializer-list)
    * Configuration - `cfg<override> = {.filter = "test"}`

  * [Non-Type Template Parameter](https://eel.is/c++draft/temp.arg.nontype)
    * Constant matchers - `constant<42_i == 42>`

  * [Template Parameter List for generic lambdas](https://eel.is/c++draft/expr.prim.lambda)
    * Parameterized tests - `"types"_test = []<class T>() {};`

  * [Concepts](https://eel.is/c++draft/concepts.lang)
    * Operators - `Operator @ Operator`

  * [Modules](https://eel.is/c++draft/module)
    * `import boost.ut;`

</p>
</details>

<a name="cpp-2x"></a>
<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;C++2X integration?</summary>
<p>

> Parameterized tests with Expansion statements (https://wg21.link/P1306r1)

```cpp
template for (auto arg : std::tuple<int, double>{}) {
  test("types " + std::to_string(arg)) = [arg] {
    expect(type(arg) == type<int> or type(arg) == type<double>);
  };
}
```

```
All tests passed (2 asserts in 2 tests)
```

> https://cppx.godbolt.org/z/dMmqmM

</p>
</details>

<a name="std"></a>
<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;Is standardization an option?</summary>
<p>

> 個人的には、C++標準はこの共通テストプリミティブ (`expect`, `""_test`)の恩恵を得られると信じている。
* 言語へのエントリーレベルを下げる（サードパーティのライブラリが不要になる）
* 標準的方法を学ぶことは、教育的効果がある
* 言語がより一貫したものになる（他の機能との一貫した設計、安定したAPI）。
* テストをファーストクラスにする（コミュニティが言語のこの側面を気にかけていることを示す）。
* 標準ライブラリ（STL）のテストを標準的な方法で公開することができる（一貫性、拡張の容易さ）。
* 特定の実装が適合しているかどうかを検証する方法として、追加ドキュメントとして機能することができる（品質、自己検証）。
* テストのための標準語彙の確立に役立つ（STL や他のプロジェクトで共通）。

</p>
</details>

<a name="macros"></a>
<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;Can I still use macros?</summary>
<p>

> はい、ただし以下のようなマクロを使うことには否定的な点もある。

* エラーメッセージが明確でなかったり、間違った行を指していたりする。
* グローバル・スコープが汚染される
* 型安全性が無視される

```cpp
#define EXPECT(...) ::boost::ut::expect(::boost::ut::that % __VA_ARGS__)
#define SUITE       ::boost::ut::suite _ = []
#define TEST(name)  ::boost::ut::detail::test{"test", name} = [=]() mutable

SUITE {
  TEST("suite") {
    EXPECT(42 == 42);
  };
};

int main() {
  TEST("macro") {
    EXPECT(1 != 2);
  };

  TEST("vector") {
    std::vector<int> v(5);

   EXPECT((5u == std::size(v)) >> fatal) << "fatal";

    TEST("resize bigger") {
      v.resize(10);
      EXPECT(10u == std::size(v));
    };
  };
}
```

```
All tests passed (4 asserts in 3 tests)
```

> https://godbolt.org/z/WcEKTr

</p>
</details>

<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;What about Mocks/Stubs/Fakes?</summary>
<p>

> 次のフレームワークのいずれかを検討してください。

* [GMock](https://github.com/cpp-testing/GUnit/blob/master/docs/GMock.md)
* [FakeIt](https://github.com/eranpeer/FakeIt)
* [hippomocks](https://github.com/dascandy/hippomocks)

</p>
</details>

<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;What about Microbenchmarking?</summary>
<p>

> [Example benchmark](example/benchmark.cpp)

> 次のフレームワークのいずれかを検討してください。

* [benchmark](https://github.com/google/benchmark)
* [Celero](https://github.com/DigitalInBlue/Celero)
* [nonius](https://github.com/libnonius/nonius)
* [nanobench](https://github.com/martinus/nanobench)

</p>
</details>

<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;Related materials/talks?</summary>
<p>

* [[Boost].UT - Unit Testing Framework - Kris Jusiak](https://boost-ext.github.io/ut/denver-cpp-2019)
* [Future of Testing with C++20 - Kris Jusiak](https://boost-ext.github.io/ut/meeting-cpp-2020)
* [Macro-Free Testing with C++20 - Kris Jusiak](https://www.youtube.com/watch?v=irdgFyxOs_Y)
* ["If you liked it then you `"should have put a"_test` on it", Beyonce rule - Kris Jusiak](https://www.youtube.com/watch?v=yCI8MjvOMeE)
* [Principles of Unit Testing With C++ - Dave Steffen and Kris Jusiak](https://www.youtube.com/watch?v=oOcuJdJJ33g)
* [Empirical Unit Testing - Dave Steffen](https://www.twitch.tv/videos/686512433)

</p>
</details>

<a name="how-to-contribute"></a>
<details open><summary>&nbsp;&nbsp;&nbsp;&nbsp;How to contribute?</summary>
<p>

> [CONTRIBUTING](.github/CONTRIBUTING.md)

</p></details>

</p>
</details>

<a name="benchmarks"></a>
<details open><summary>Benchmarks</summary>
<p>

| Framework | Version | Standard | License | Linkage | Test configuration |
|-|-|-|-|-|-|
| [Boost.Test](https://github.com/boostorg/test) | [1.71.0](https://www.boost.org/users/history/version_1_71_0.html) | C++03 | Boost 1.0 | single header/library | `static library` |
| [GoogleTest](https://github.com/google/googletest) | [1.10.0](https://github.com/google/googletest/releases/tag/release-1.10.0) | C++11 | BSD-3 | library | `static library` |
| [Catch](https://github.com/catchorg/Catch2) | [2.10.2](https://github.com/catchorg/Catch2/releases/download/v2.10.2/catch.hpp) | C++11 | Boost 1.0 | single header | `CATCH_CONFIG_FAST_COMPILE` |
| [Doctest](https://github.com/onqtam/doctest) | [2.3.5](https://github.com/onqtam/doctest/blob/master/doctest/doctest.h) | C++11 | MIT | single header | `DOCTEST_CONFIG_SUPER_FAST_ASSERTS` |
| [UT](https://github.com/boost-ext/ut) | [1.1.0](https://github.com/boost-ext/ut/blob/master/include/boost/ut.hpp) | C++20 | Boost 1.0 | single header/module | |

<table>
  <tr>
    <td colspan="3" align="center">
    <a href="https://github.com/cpp-testing/ut-benchmark/tree/master/benchmarks"><b>Include</b></a> / <i>0 tests, 0 asserts, 1 cpp file</i>
    </td>
  </tr>
  <tr>
    <td colspan="3" align="center"><a href="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/Compilation_include.png"><img src="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/Compilation_include.png"></a></td>
    <td></td>
  </tr>

  <tr>
    <td colspan="3" align="center">
    <a href="https://github.com/cpp-testing/ut-benchmark/tree/master/benchmarks"><b>Assert</b></a> / <i>1 test, 1'000'000 asserts, 1 cpp file</i>
    </td>
  </tr>
  <tr>
    <td><a href="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/Compilation_assert.png"><img src="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/Compilation_assert.png"></a></td>
    <td><a href="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/Execution_assert.png"><img src="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/Execution_assert.png"></a></td>
    <td><a href="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/BinarySize_assert.png"><img src="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/BinarySize_assert.png"></a></td>
  </tr>

  <tr>
    <td colspan="3" align="center">
    <a href="https://github.com/cpp-testing/ut-benchmark/tree/master/benchmarks"><b>Test</b></a> / <i>1'000 tests, 0 asserts, 1 cpp file</i>
    </td>
  </tr>
  <tr>
    <td><a href="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/Compilation_test.png"><img src="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/Compilation_test.png"></a></td>
    <td><a href="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/Execution_test.png"><img src="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/Execution_test.png"></a></td>
    <td><a href="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/BinarySize_test.png"><img src="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/BinarySize_test.png"></a></td>
  </tr>

  <tr>
    <td colspan="3" align="center">
    <a href="https://github.com/cpp-testing/ut-benchmark/tree/master/benchmarks"><b>Suite</b></a> / <i>10'000 tests, 0 asserts, 100 cpp files</i>
    </td>
  </tr>
  <tr>
    <td><a href="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/Compilation_suite.png"><img src="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/Compilation_suite.png"></a></td>
    <td><a href="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/Execution_suite.png"><img src="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/Execution_suite.png"></a></td>
    <td><a href="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/BinarySize_suite.png"><img src="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/BinarySize_suite.png"></a></td>
  </tr>

  <tr>
    <td colspan="3" align="center">
    <a href="https://github.com/cpp-testing/ut-benchmark/tree/master/benchmarks"><b>Suite+Assert</b></a> / <i>10'000 tests, 40'000 asserts, 100 cpp files</i>
    </td>
  </tr>
  <tr>
    <td><a href="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/Compilation_suite+assert.png"><img src="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/Compilation_suite+assert.png"></a></td>
    <td><a href="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/Execution_suite+assert.png"><img src="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/Execution_suite+assert.png"></a></td>
    <td><a href="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/BinarySize_suite+assert.png"><img src="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/BinarySize_suite+assert.png"></a></td>
  </tr>

  <tr>
    <td colspan="3" align="center">
    <a href="https://github.com/cpp-testing/ut-benchmark/tree/master/benchmarks"><b>Suite+Assert+STL</b></a> / <i>10'000 tests, 20'000 asserts, 100 cpp files</i>
    </td>
  </tr>
  <tr>
    <td><a href="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/Compilation_suite+assert+stl.png"><img src="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/Compilation_suite+assert+stl.png"></a></td>
    <td><a href="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/Execution_suite+assert+stl.png"><img src="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/Execution_suite+assert+stl.png"></a></td>
    <td><a href="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/BinarySize_suite+assert+stl.png"><img src="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/BinarySize_suite+assert+stl.png"></a></td>
  </tr>

  <tr>
    <td colspan="3" align="center">
    <a href="https://github.com/cpp-testing/ut-benchmark/tree/master/benchmarks"><b>Incremental Build - Suite+Assert+STL</b></a> / <i>1 cpp file change (1'000 tests, 20'000 asserts, 100 cpp files)</i>
    </td>
  </tr>
  <tr>
    <td><a href="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/Compilation_incremental.suite+assert+stl.png"><img src="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/Compilation_incremental.suite+assert+stl.png"></a></td>
    <td><a href="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/Execution_incremental.suite+assert+stl.png"><img src="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/Execution_incremental.suite+assert+stl.png"></a></td>
    <td><a href="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/BinarySize_incremental.suite+assert+stl.png"><img src="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/BinarySize_incremental.suite+assert+stl.png"></a></td>
  </tr>

  <tr>
    <td colspan="3" align="center">
    <a href="https://github.com/cpp-testing/ut-benchmark/tree/master/benchmarks"><b>Suite+Assert+STL</b></a> / <i>10'000 tests, 20'000 asserts, 100 cpp files<br/>(Headers vs Precompiled headers vs C++20 Modules)</i>
    </td>
  </tr>
  <tr>
    <td><a href="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/ut_Compilation_suite+assert+stl.png"><img src="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/ut_Compilation_suite+assert+stl.png"></a></td>
    <td><a href="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/ut_Execution_suite+assert+stl.png"><img src="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/ut_Execution_suite+assert+stl.png"></a></td>
    <td><a href="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/ut_BinarySize_suite+assert+stl.png"><img src="https://raw.githubusercontent.com/cpp-testing/ut-benchmark/master/results/ut_BinarySize_suite+assert+stl.png"></a></td>
  </tr>
</table>

> https://github.com/cpp-testing/ut-benchmark

</p>
</details>

</p>
</details>

---

**免責事項** `UT` はBoostの公式ライブラリではありません。

<p align="left"><img width="5%" src="https://github.com/boost-ext/ut/raw/gh-pages/images/logo.png" /></p>