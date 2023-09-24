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




