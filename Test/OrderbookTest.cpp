#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>

#include "InputHandler.hpp"
#include "Order.hpp"
#include "Orderbook.hpp"

namespace googletest = ::testing;

int main(int argc, char **argv) {
  googletest::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

class OrderbookTestFixture : public googletest::TestWithParam<const char *> {
private:
  const static inline std::filesystem::path Parent{
      std::filesystem::current_path().parent_path()};

public:
  const static inline std::filesystem::path TestFolderPath{Parent};
};

TEST_P(OrderbookTestFixture, OrderbookTestSuite) {
  const auto inputFile = OrderbookTestFixture::TestFolderPath /
                         (std::string(GetParam()) + "_input.txt");
  const auto resultFile = OrderbookTestFixture::TestFolderPath /
                          (std::string(GetParam()) + "_result.txt");
  InputHandler handler;
  Orderbook ob;
  TradePointers trades;
  auto orders = handler.ParseInputFile(inputFile);
  auto [expectedTradePointers, expectedRemOrders] =
      handler.ParseResultFile(resultFile);
  for (const auto &order : orders) {
    ob.AddAndMatchOrder(order, trades);
  }
  ASSERT_EQ(expectedTradePointers.size(), trades.size());
  ASSERT_EQ(expectedRemOrders.size(), ob.GetUnmatchedOrderCount());
  for (size_t i = 0; i < expectedTradePointers.size(); ++i) {
    ASSERT_TRUE(expectedTradePointers[i]->AreFieldsEqual(trades[i]));
  }
  for (size_t i = 0; i < expectedRemOrders.size(); ++i) {
    auto o = ob.GetUnfilledOrder(i);
    ASSERT_NE(o, nullptr);
    auto res = expectedRemOrders[i]->AreFieldsEqual(o);
    ASSERT_TRUE(res);
  }
}

INSTANTIATE_TEST_SUITE_P(
    Tests, OrderbookTestFixture,
    googletest::ValuesIn({"basic", "fully_fill", "nothing_fill",
                          "one_bid_multiple_ask", "one_ask_multiple_bid",
                          "unknown_side", "invalid_quantity",
                          "invalid_price"}));