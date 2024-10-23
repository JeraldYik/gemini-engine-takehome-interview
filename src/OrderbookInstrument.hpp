#pragma once

#include "Order.hpp"
#include <map>

// OrderPointers will be naturally arranged by timestamp due to nature of
// insertion.
using BidOrder = std::map<Price, OrderPointers, std::greater<Price>>;
using AskOrder = std::map<Price, OrderPointers, std::less<Price>>;

class OrderbookInstrument {
public:
  OrderbookInstrument() : bidOrders_(), askOrders_() {};

  BidOrder &GetBidOrders() { return bidOrders_; };

  AskOrder &GetAskOrders() { return askOrders_; };

  bool IsAnyEmpty() const noexcept {
    return bidOrders_.empty() || askOrders_.empty();
  };

private:
  BidOrder bidOrders_;
  AskOrder askOrders_;
};