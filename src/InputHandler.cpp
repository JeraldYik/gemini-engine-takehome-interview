#include <charconv>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>
#include <vector>

#include "InputHandler.hpp"
#include "Order.hpp"
#include "Orderbook.hpp"

// numExpectedElements = 0 indicates unknown number of elements after split,
// referring to the dynamic nature of input
std::vector<std::string>
InputHandler::SplitString(const std::string &str, char delimiter,
                          uint32_t numExpectedElements) const noexcept {
  std::vector<std::string> elements;
  if (numExpectedElements != 0) {
    elements.reserve(numExpectedElements);
  }
  size_t count = 0;
  size_t start = 0;
  while (start < str.size()) {
    while (start < str.size() && str[start] == delimiter)
      start++;
    size_t end = start;
    while (end < str.size() && str[end] != delimiter)
      end++;
    if (start < end) {
      auto element = str.substr(start, end - start);
      elements.emplace_back(element);
      count++;
    }

    if (numExpectedElements != 0 && count == numExpectedElements)
      return elements;
    start = end;
  }
  return elements;
}

std::uint32_t InputHandler::ToNumber(const std::string_view &str) const {
  std::int64_t value{};
  std::from_chars(
      str.data(), str.data() + str.size(),
      value); // not using std::stoi to avoid creating copy of string
  if (value < 0)
    throw std::runtime_error("Negative number");
  return static_cast<std::uint32_t>(value);
}

OrderId InputHandler::ToOrderId(const std::string_view &str) const {
  if (str.empty())
    throw std::runtime_error("Empty OrderId");
  return static_cast<OrderId>(str);
}

Side InputHandler::ToSide(const std::string_view &str) const {
  if (str.empty())
    throw std::runtime_error("Empty Side");
  if (str == "BUY")
    return Side::Buy;
  if (str == "SELL")
    return Side::Sell;
  throw std::runtime_error("Unknown Side");
}

Instrument InputHandler::ToInstrument(const std::string_view &str) const {
  if (str.empty())
    throw std::runtime_error("Empty Instrument");
  return static_cast<Instrument>(str);
}

Quantity InputHandler::ToQuantity(const std::string_view &str) const {
  if (str.empty())
    throw std::runtime_error("Empty Quantity");
  return static_cast<Quantity>(ToNumber(str));
}

Price InputHandler::ToPrice(const std::string_view &str) const {
  if (str.empty())
    throw std::runtime_error("Empty Price");
  return static_cast<Price>(ToNumber(str));
}

std::vector<std::string>
InputHandler::ParsePipedInput(const std::string &input) const noexcept {
  return InputHandler::SplitString(input, '@', 0);
}

// see basic_input.txt for example format
OrderPointer InputHandler::ParseOrderLine(const std::string &line) const {
  auto elements = SplitString(line, ' ', 5);
  if (elements.size() != 5) {
    throw std::runtime_error(
        "Input line is malformed. Does not contain 5 elements.");
  }
  auto orderId = ToOrderId(elements[0]);
  auto side = ToSide(elements[1]);
  auto instrument = ToInstrument(elements[2]);
  auto quantity = ToQuantity(elements[3]);
  auto price = ToPrice(elements[4]);
  OrderPointer order =
      std::make_shared<Order>(orderId, side, instrument, quantity, price);
  return order;
}

std::vector<OrderPointer>
InputHandler::ParseInputFile(const std::filesystem::path &path) const noexcept {
  std::vector<OrderPointer> orders;

  std::string line;
  std::ifstream file{path};
  while (std::getline(file, line)) {
    if (line.empty())
      break;

    try {
      auto order = ParseOrderLine(line);
      orders.emplace_back(order);
    } catch (const std::runtime_error e) {
      std::cerr << "Error parsing line: " << line << " - " << e.what()
                << std::endl;
    }
  }

  return orders;
}

// see basic_result.txt for example format
TradePointer InputHandler::ParseTradeLine(const std::string &line) const {
  auto elements = SplitString(line, ' ', 6);
  if (elements.size() != 6) {
    throw std::runtime_error(
        "Input line is malformed. Does not contain 6 elements.");
  }
  if (elements[0] != "TRADE") {
    throw std::runtime_error(
        "Input line is malformed. Does not start with 'TRADE'");
  }
  auto instrument = ToInstrument(elements[1]);
  auto askOrderId = ToOrderId(elements[2]);
  auto bidOrderId = ToOrderId(elements[3]);
  auto quantity = ToQuantity(elements[4]);
  auto price = ToPrice(elements[5]);
  TradePointer trade = std::make_shared<Trade>(instrument, askOrderId,
                                               bidOrderId, quantity, price);
  return trade;
}

std::pair<TradePointers, std::vector<OrderPointer>>
InputHandler::ParseResultFile(const std::filesystem::path &path) const {
  TradePointers trades;
  std::vector<OrderPointer> orders;

  std::string line;
  std::ifstream file{path};
  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }

    try {
      auto trade = InputHandler::ParseTradeLine(line);
      trades.emplace_back(trade);
    } catch (const std::exception e) {
      std::cerr << "Error in parsing line to trade - " << e.what() << std::endl;
    }
    try {
      auto order = InputHandler::ParseOrderLine(line);
      orders.emplace_back(order);
    } catch (const std::exception e) {
      std::cerr << "Error in parsing line to order - " << e.what() << std::endl;
    }
  }
  return std::pair<TradePointers, std::vector<OrderPointer>>(trades, orders);
}