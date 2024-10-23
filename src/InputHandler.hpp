#pragma once

#include <charconv>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string_view>
#include <vector>

#include "Order.hpp"
#include "Orderbook.hpp"
#include "Trade.hpp"

struct InputHandler {
  std::vector<std::string>
  SplitString(const std::string &str, char delimiter,
              uint32_t numExpectedElements) const noexcept;

  std::uint32_t ToNumber(const std::string_view &str) const;

  OrderId ToOrderId(const std::string_view &str) const;

  Side ToSide(const std::string_view &str) const;

  Instrument ToInstrument(const std::string_view &str) const;

  Quantity ToQuantity(const std::string_view &str) const;

  Price ToPrice(const std::string_view &str) const;

  std::vector<std::string>
  ParsePipedInput(const std::string &input) const noexcept;

  OrderPointer ParseOrderLine(const std::string &line) const;

  std::vector<OrderPointer>
  ParseInputFile(const std::filesystem::path &path) const noexcept;

  TradePointer ParseTradeLine(const std::string &line) const;

  std::pair<TradePointers, std::vector<OrderPointer>>
  ParseResultFile(const std::filesystem::path &path) const;
};