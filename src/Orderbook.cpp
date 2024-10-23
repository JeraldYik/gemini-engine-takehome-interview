#include <iostream>
#include <memory>
#include <stdio.h>

#include "InputHandler.hpp"
#include "Order.hpp"
#include "Orderbook.hpp"
#include "OrderbookInstrument.hpp"
#include "Side.hpp"

void Orderbook::Orderbook::AddAndMatchOrder(OrderPointer order,
                                            TradePointers &m_trades) {
  this->AddOrder(order);
  this->MatchOrder(order->GetInstrument(), m_trades);
}

// note: it is possible to enhance this syntax with overloading [] operator
void Orderbook::AddOrder(OrderPointer order) noexcept {
  if (orderBook_.find(order->GetInstrument()) == orderBook_.end()) {
    orderBook_[order->GetInstrument()] = OrderbookInstrument();
  }
  if (order->GetSide() == Side::Buy) {
    auto &bidOrders = orderBook_[order->GetInstrument()].GetBidOrders();
    if (bidOrders.find(order->GetPrice()) == bidOrders.end()) {
      bidOrders[order->GetPrice()] = OrderPointers();
    }
    orderBook_[order->GetInstrument()]
        .GetBidOrders()[order->GetPrice()]
        .emplace_back(order);

    unfilledBidOrders_.emplace(order->GetTimestamp(), order);
  } else {
    auto &askOrders = orderBook_[order->GetInstrument()].GetAskOrders();
    if (askOrders.find(order->GetPrice()) == askOrders.end()) {
      askOrders[order->GetPrice()] = OrderPointers();
    }
    orderBook_[order->GetInstrument()]
        .GetAskOrders()[order->GetPrice()]
        .emplace_back(order);

    unfilledAskOrders_.emplace(order->GetTimestamp(), order);
  }
};

void Orderbook::MatchOrder(const Instrument &instrument,
                           TradePointers &m_trades) {
  while (!orderBook_[instrument].IsAnyEmpty()) {
    auto &[bidPrice, bids] = *orderBook_[instrument].GetBidOrders().begin();
    auto &[askPrice, asks] = *orderBook_[instrument].GetAskOrders().begin();
    if (bidPrice < askPrice)
      break; // No match possible

    // Match found
    while (!bids.empty() && !asks.empty()) {
      auto bid = bids.front();
      auto ask = asks.front();
      auto minQuantity = std::min(bid->GetQuantity(), ask->GetQuantity());
      m_trades.emplace_back(std::make_shared<Trade>(
          instrument, ask->GetOrderId(), bid->GetOrderId(), minQuantity,
          bid->GetPrice()));

      // Adjust quantities
      bid->Fill(minQuantity);
      ask->Fill(minQuantity);

      // Remove orders if fully executed
      if (bid->IsFilled()) {
        bids.pop_front();
        unfilledBidOrders_.erase(bid->GetTimestamp());
      }
      if (ask->IsFilled()) {
        asks.pop_front();
        unfilledAskOrders_.erase(ask->GetTimestamp());
      }
    }

    if (bids.empty()) {
      orderBook_[instrument].GetBidOrders().erase(bidPrice);
    }
    if (asks.empty()) {
      orderBook_[instrument].GetAskOrders().erase(askPrice);
    }
  }
}

void Orderbook::PrintTradePointers(const TradePointers &trades) noexcept {
  for (auto &trade : trades) {
    trade->Print();
  }
  if (trades.size() > 0)
    std::cout << std::endl;
}

void Orderbook::PrintUnmatchedOrder() noexcept {
  for (auto &[_, o] : unfilledAskOrders_) {
    std::cout << o->GetOrderId() << " SELL " << o->GetInstrument() << " "
              << o->GetQuantity() << " " << o->GetPrice() << std::endl;
  }
  for (auto &[_, o] : unfilledBidOrders_) {
    std::cout << o->GetOrderId() << " BUY " << o->GetInstrument() << " "
              << o->GetQuantity() << " " << o->GetPrice() << std::endl;
  }
}

// Attempt to get ask orders first, then bid orders
OrderPointer Orderbook::GetUnfilledOrder(size_t i) noexcept {
  if (i > this->GetUnmatchedOrderCount())
    return nullptr;
  if (i < this->GetUnfilledAskOrders().size()) {
    return this->GetUnfilledAskOrder(i);
  }
  i -= this->GetUnfilledAskOrders().size();
  if (i < this->GetUnfilledBidOrders().size()) {
    return this->GetUnfilledBidOrder(i);
  }
  return nullptr;
};

OrderPointer Orderbook::GetUnfilledBidOrder(size_t i) noexcept {
  if (i >= unfilledBidOrders_.size()) {
    return nullptr;
  }
  auto it = unfilledBidOrders_.begin();
  std::advance(it, i);
  if (it == unfilledBidOrders_.end()) {
    return nullptr;
  }
  return it->second;
}

OrderPointer Orderbook::GetUnfilledAskOrder(size_t i) noexcept {
  if (i >= unfilledAskOrders_.size()) {
    return nullptr;
  }
  auto it = unfilledAskOrders_.begin();
  std::advance(it, i);
  if (it == unfilledAskOrders_.end()) {
    return nullptr;
  }
  return it->second;
}

Timestamp Order::Order::s_timestamp_ = static_cast<Timestamp>(1);