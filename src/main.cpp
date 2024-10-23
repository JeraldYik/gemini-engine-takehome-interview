#include <iostream>
#include <memory>

#include "InputHandler.cpp"
#include "Order.hpp"
#include "Orderbook.hpp"
#include "Trade.hpp"

int main() {
  // std::cerr << "====== Match Engine =====" << std::endl;
  std::string input;
  Orderbook ob;
  InputHandler handler;
  TradePointers trades;
  std::getline(std::cin, input);
  // std::cerr << "Input: " << input << std::endl;
  if (input == "") {
    std::cerr << "Input is empty" << std::endl;
    return 0;
  }
  auto lines = handler.ParsePipedInput(input);
  for (auto &line : lines) {
    OrderPointer order;
    try {
      order = handler.ParseOrderLine(line);
    } catch (const std::exception e) {
      std::cerr << "Error parsing line: " << line << " - " << e.what()
                << std::endl;
      continue;
    }

    try {
      ob.AddAndMatchOrder(order, trades);
    } catch (const std::exception e) {
      std::cerr << "Error adding and matching order: " << e.what() << std::endl;
      continue;
    }
  }
  // although the requirement states to print any trades during the streaming
  // process, the order in which they are printed to stdout would be the same as
  // if they are collected during the stream, and then printed after the stream
  // is fully consumed.
  ob.PrintTradePointers(trades);
  ob.PrintUnmatchedOrder();
  return 0;
}
