#pragma once

#include <iterator>
#include <unordered_map>

#include "Order.hpp"
#include "OrderbookInstrument.hpp"
#include "Trade.hpp"

class Orderbook {
public:
  Orderbook() : orderBook_(), unfilledBidOrders_(), unfilledAskOrders_() {};

  // ~Orderbook();

  Orderbook(const Orderbook &) = delete;

  void operator=(const Orderbook &) = delete;

  Orderbook(Orderbook &&) = delete;

  void operator=(Orderbook &&) = delete;

  void AddAndMatchOrder(OrderPointer order, TradePointers &trades);

  void AddOrder(OrderPointer order) noexcept;

  void MatchOrder(const Instrument &instrument, TradePointers &trades);

  void PrintTradePointers(const TradePointers &trades) noexcept;

  void PrintUnmatchedOrder() noexcept;

  size_t GetUnmatchedOrderCount() const noexcept {
    return unfilledAskOrders_.size() + unfilledBidOrders_.size();
  }

  OrderPointer GetUnfilledOrder(size_t i) noexcept;

  OrderOnTimestamp GetUnfilledBidOrders() noexcept {
    return unfilledBidOrders_;
  }

  OrderOnTimestamp GetUnfilledAskOrders() noexcept {
    return unfilledAskOrders_;
  }

  OrderPointer GetUnfilledBidOrder(size_t i) noexcept;

  OrderPointer GetUnfilledAskOrder(size_t i) noexcept;

private:
  std::unordered_map<Instrument, OrderbookInstrument> orderBook_;
  OrderOnTimestamp unfilledBidOrders_;
  OrderOnTimestamp unfilledAskOrders_;
};