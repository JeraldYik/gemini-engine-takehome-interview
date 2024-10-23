#pragma once

#include <list>
#include <map>
#include <memory>

#include "Side.hpp"

using Timestamp = std::uint32_t;
using OrderId = std::string;
using Instrument = std::string;
using Quantity = std::uint32_t;
using Price = std::uint32_t;

class Order {
public:
  Order(OrderId orderId, Side side, Instrument instrument, Quantity quantity,
        Price price)
      : timestamp_{s_timestamp_}, orderId_{orderId}, side_{side},
        instrument_{instrument}, quantity_{quantity}, price_{price} {
    s_timestamp_++;
  }

  OrderId GetOrderId() const noexcept { return orderId_; }
  Side GetSide() const noexcept { return side_; }
  Instrument GetInstrument() const noexcept { return instrument_; }
  Quantity GetQuantity() const noexcept { return quantity_; }
  Price GetPrice() const noexcept { return price_; }
  Timestamp GetTimestamp() const noexcept { return timestamp_; }
  void Fill(const Quantity &quantity) {
    if (quantity > quantity_)
      throw std::logic_error(
          "Order cannot be filled as quantity deducted is invalid");
    quantity_ -= quantity;
  }
  bool IsFilled() const noexcept { return quantity_ == 0; }

  bool AreFieldsEqual(const std::shared_ptr<Order> other) const noexcept {
    return orderId_ == other->GetOrderId() && side_ == other->GetSide() &&
           instrument_ == other->GetInstrument() &&
           quantity_ == other->GetQuantity() && price_ == other->GetPrice();
  }

private:
  static Timestamp s_timestamp_;
  Timestamp timestamp_;
  OrderId orderId_;
  Side side_;
  Instrument instrument_;
  Quantity quantity_;
  Price price_;
};

using OrderPointer = std::shared_ptr<Order>;
using OrderPointers = std::list<OrderPointer>; // doubly-linked list
using OrderOnTimestamp =
    std::map<Timestamp, OrderPointer, std::less<Timestamp>>;
