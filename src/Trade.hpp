#pragma once

#include "Order.hpp"
#include <iostream>
#include <memory>
#include <vector>

class Trade {
public:
  Trade(Instrument instrument, OrderId askOrderId, OrderId bidOrderId,
        Quantity quantity, Price price)
      : instrument_{instrument}, askOrderId_{askOrderId},
        bidOrderId_{bidOrderId}, quantity_{quantity}, price_{price} {}

  void Print() const noexcept {
    std::cout << "TRADE " << instrument_ << " " << askOrderId_ << " "
              << bidOrderId_ << " " << quantity_ << " " << price_ << std::endl;
  }

  bool AreFieldsEqual(const std::shared_ptr<Trade> other) const noexcept {
    return instrument_ == other->instrument_ &&
           askOrderId_ == other->askOrderId_ &&
           bidOrderId_ == other->bidOrderId_ && quantity_ == other->quantity_ &&
           price_ == other->price_;
  }

private:
  Instrument instrument_;
  OrderId askOrderId_;
  OrderId bidOrderId_;
  Quantity quantity_;
  Price price_;
};

using TradePointer = std::shared_ptr<Trade>;
using TradePointers = std::vector<TradePointer>;