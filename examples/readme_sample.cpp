/* boost-ext/ut/README.jpに記載のサンプルコードをまとめる */
// 現在、 `L636` までのコードをまとめている

#include <boost/ut.hpp>
// import boost.ut

int main() {
  {
    // first assertion
    boost::ut::expect(true);
    boost::ut::expect(1 == 2);
  }
  {
    /* アサーション */
    using namespace boost::ut;
    expect(1_i == 2);                 // UDL(User Defined Literal) syntax
    expect(1 == 2_i);                 // UDL syntax
    expect(that % 1 == 2);            // Matcher syntax (`1_i == 2`と同等)
    expect(eq(1, 2));                 // eq/neq/gt/lt/ge/le
    expect(42l == 42_l and 1 == 2_i); // compound expression (and/or/not)
    expect(42.10000001 == 42.1_d) << "epsilon=0.1"; // floating point comparison

    /* カスタムメッセージ と fatal動作 */
    using namespace boost::ut;
    expect(42l == 42_l and 1 == 2_i) << "additional info"; // custom message
    expect((1 == 1_i) >> fatal);                           // fatal assertion
    expect(1_i == 2); // If the previous line fails, not executed
    expect(1 == 1_i) << "fatal assertion" << fatal;

    /* ログの出力 */
    /* std::formatは開発環境で未対応のためコメントアウト */
    // #include <format>
    "logging"_test = [] {
      // log("\npre  {} == {}\n", 42, 43);
      log << "pre";
      expect(42_i == 43) << "message on failure";
      // log("\npost {} == {} -> {}\n", 42, 43, 42 == 43);
      log << "post";
    };

    /* std::expected (C++23) の使用方法 */
    /* 開発環境下で未対応のため、コメントアウト */
    // #include <expected>
    // using namespace boost::ut;
    // "lazy log"_test = [] {
    //   std::expected<bool, std::string> e = std::unexpected("lazy evaluated");
    //   expect(e.has_value()) << [&] { return e.error(); } << fatal;
    //   expect(e.value() == true);
    // };
  }
  {
    /* expectを省略した簡易表記法 */
    using namespace boost::ut::literals;
    using namespace boost::ut::operators::terse;
    1_i == 2; // terse notation
  }
  {
    /* Test cases（グループ化）*/
    using namespace boost::ut;
    "hello world"_test = [] {
      int i = 43;
      expect(42_i == i);
    };
    test("hello world") = [] {}; // こちらはフォーマット文字列を使用可能

    /* テストケース内で setup を共有する */
    "[vector]"_test = [] {
      std::vector<int> v(5); // setup
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
    };

    /* パラメタ化されたテスト */
    for (auto i : std::vector{1, 2, 3}) {
      test("parameterized " + std::to_string(i)) = [i] { // 3 tests
        expect(that % i > 0);                            // 3 asserts
      };
    }
    "types with type name"_test = []<class T>() {
      expect(std::is_unsigned_v<T>)
          << reflection::type_name<T>() << "is unsigned";
    } | std::tuple<unsigned int, float>{};

    /* パラメタの後置 */
    "args"_test = [](const auto &arg) {
      expect(arg > 0_i) << "all values greater than 0";
    } | std::vector{1, 2, 3};

    /* テストのスキップ */
    skip / test("don't run function") = [] {
      expect(42_i == 43) << "should not fire!";
    };

    /* タグの付与 */
    tag("nightly") / tag("slow") /
        "performance"_test = [] { expect(42_i == 42); };
    /* タグによる実行フィルタ */
    cfg<override> = {.filter = "tag", .tag = {"execute"}};
    tag("not executed") / "tag"_test = [] { expect(43_i == 42); };
  }
  {
    namespace ut = boost::ut;
    /* Test Suites (テストケースのグループ) の登録 */
    ut::suite errors = [] {
      using namespace ut;

      "exception"_test = [] {
        expect(throws([] { throw 0; })) << "throws any exception";
      };

      "failure"_test = [] { expect(nothrow([] {})); };
    };
  }
  {
    using namespace boost::ut;
    using namespace boost::ut::bdd;
    /* BDD構文 (given, when, then) */
    "vector"_test = [] {
      given("I have a vector") = [] {
        std::vector<int> v(6);
        expect((5_ul == std::size(v)) >> fatal);

        when("I resize bigger") = [=] {
          mut(v).resize(10);

          then("The size should increase") = [=] {
            expect(10_ul == std::size(v));
          };
          then("The size should increase") = [=] {
            expect(std::size(v) == 10_ul);
          };
        };
      };
    };

    /* BDD + feature/scenario */
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

    /* Gherking記法 (feature, scenario, given, when, then) */
    bdd::gherkin::steps steps = [](auto &steps) {
      steps.feature("Vector") = [&] {
        steps.scenario("*") = [&] {
          steps.given("I have a vector") = [&] {
            std::vector<int> v(5);
            expect((5_ul == std::size(v)) >> fatal);

            steps.when("I resize bigger") = [&] { v.resize(10); };

            steps.then("The size should increase") = [&] {
              expect(10_ul == std::size(v));
            };
          };
        };
      };
    };

    /* Spec記法 (describe, it) */
    /* contextはサポートされていない */
    using namespace boost::ut::spec;
    describe("vector") = [] {
      std::vector<int> v(5);
      expect((5_ul == std::size(v)) >> fatal);

      it("should resize bigger") = [v] {
        mut(v).resize(10);
        expect(10_ul == std::size(v));
      };
    };
  }
  {}
}