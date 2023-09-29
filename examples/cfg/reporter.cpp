//
// Copyright (c) 2019-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/ut.hpp>
#include <string_view>

namespace ut = boost::ut;

namespace cfg {
class reporter {
 public:
  auto on(ut::events::test_begin) -> void {
    std::cout << "test begin\n";
  }
  auto on(ut::events::test_run) -> void {
    std::cout << "test run\n";
  }
  auto on(ut::events::test_skip) -> void {
    std::cout << "test skip\n";
  }
  auto on(ut::events::test_end) -> void {
    std::cout << "test end\n";
  }
  template <class TMsg>
  auto on(ut::events::log<TMsg> log) -> void {
    std::cout << "log " << log.msg << '\n';
  }
  template <class TExpr>
  auto on(ut::events::assertion_pass<TExpr> event) -> void {
    std::cout << "assertion pass\n";
  }
  template <class TExpr>
  auto on(ut::events::assertion_fail<TExpr>) -> void {
    std::cout << "assertion fail\n";
  }
  auto on(ut::events::fatal_assertion) -> void {
    std::cout << "fatal assertion\n";
  }
  auto on(ut::events::exception) -> void {
    std::cout << "exception\n";
  }
  auto on(ut::events::summary event) -> void {
    std::cout << "summary\n";
  }
};
}  // namespace cfg

template <>
auto ut::cfg<ut::override> = ut::runner<cfg::reporter>{};

int main() {
  using namespace ut;
  using namespace std::literals::string_view_literals;

  "example"_test = [] {
    expect(42 == 42_i);
    expect("ut"sv != "tu"sv);
  };

  // event line:
  // test_begin
  // assert_pass
  // assert_pass
  // test_end
  // summary
}