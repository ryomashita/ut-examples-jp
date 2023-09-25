/* boost-ext/ut/README.jpに記載のサンプルコードをまとめる */
// 現在、 `L370` までのコードをまとめている

#include <boost/ut.hpp>
// import boost.ut

int main(){
    // first assertion
    boost::ut::expect(true);
    boost::ut::expect(1 == 2);

    {
        using namespace boost::ut;
        expect(1_i == 2); // UDL(User Defined Literal) syntax
        expect(1 == 2_i); // UDL syntax
        expect(that % 1 == 2); // Natcher syntax
        expect(eq(1, 2)); // eq/neq/gt/lt/ge/le
        expect(42l == 42_l and 1 == 2_i); // compound expression (and/or/not)

    }
    {
        using namespace boost::ut;
        expect(42l == 42_l and 1 == 2_i) << "additional info"; // custom message
        expect((1 == 1_i) >> fatal); // fatal assertion
        expect(1_i == 2);            // If the previous line fails, not executed
        expect(1 == 1_i) << "fatal assertion" << fatal;
    }
    {
        using namespace boost::ut::literals;
        using namespace boost::ut::operators::terse;
        1_i == 2; // terse notation
    }
    {

    }

}