#include <matcher.h>
#include <order_book.h>
#include <sys/_types/_int64_t.h>

#include <deque>
#include <external/json.hpp>
#include <functional>
#include <iostream>
#include <optional>
#include <ostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

double matcher() {
  std::priority_queue<Order, std::vector<Order>, Compare> PQ = Order_Book();
  std::unordered_map<int64_t, std::deque<Order>> SellBucket;
  std::unordered_map<int64_t, std::deque<Order>> BuyBucket;
  std::priority_queue<int, std::vector<int>, std::greater<int>> SellPrices;
  std::priority_queue<int> BuyPrices;

  int HowManyMatched;
  int MarketOrderCancelled;
  int PartialOrders;

  auto start = std::chrono::steady_clock::now();

  while (!PQ.empty()) {
    Order Match = PQ.top();
    PQ.pop();

    std::optional<int64_t> RawPrice = Match.price;
    std::string Side = Match.side;

    if (RawPrice != std::nullopt) {
      if (Side == "BUY") {
        int64_t Price = RawPrice.value();

        while (Match.Qty > 0 && !SellPrices.empty()) {
          int BestPrice = SellPrices.top();

          if (BestPrice > Price) {
            break;
          }

          Order& SellOrder = SellBucket[BestPrice].front();

          int TradeQty = std::min(Match.Qty, SellOrder.Qty);

          Match.Qty -= TradeQty;
          SellOrder.Qty -= TradeQty;

          if (SellOrder.Qty == 0) {
            SellBucket[BestPrice].pop_front();
          }

          if (SellBucket[BestPrice].empty()) {
            SellBucket.erase(BestPrice);
            SellPrices.pop();
          }
        }

        if (Match.Qty > 0) {
          auto [it, Inserted] = BuyBucket.try_emplace(Price);

          if (Inserted) {
            BuyPrices.push(Price);
          }

          it->second.push_back(Match);
        }

      } else {
        int64_t Price = RawPrice.value();

        while (Match.Qty > 0 && !BuyPrices.empty()) {
          int BestPrice = BuyPrices.top();

          if (BestPrice < Price) {
            break;
          }

          Order& BuyOrder = BuyBucket[BestPrice].front();

          int TradeQty = std::min(Match.Qty, BuyOrder.Qty);

          Match.Qty -= TradeQty;
          BuyOrder.Qty -= TradeQty;

          if (BuyOrder.Qty == 0) {
            BuyBucket[BestPrice].pop_front();
          }

          if (BuyBucket[BestPrice].empty()) {
            BuyBucket.erase(BestPrice);
            BuyPrices.pop();
          }
        }

        if (Match.Qty > 0) {
          auto [it, Inserted] = SellBucket.try_emplace(Price);

          if (Inserted) {
            SellPrices.push(Price);
          }

          it->second.push_back(Match);
        }
      }

    } else {
      if (Side == "BUY") {
        while (Match.Qty > 0 && !SellPrices.empty()) {
          int BestPrice = SellPrices.top();

          Order& SellOrder = SellBucket[BestPrice].front();

          int TradedQty = std::min(Match.Qty, SellOrder.Qty);

          Match.Qty -= TradedQty;
          SellOrder.Qty -= TradedQty;

          if (SellOrder.Qty == 0) {
            SellBucket[BestPrice].pop_front();
          }

          if (SellBucket[BestPrice].empty()) {
            SellBucket.erase(BestPrice);
            SellPrices.pop();
          }
        }

      } else {
        while (Match.Qty > 0 && !BuyPrices.empty()) {
          int BestPrice = BuyPrices.top();

          Order& BuyOrder = BuyBucket[BestPrice].front();

          int TradedQty = std::min(Match.Qty, BuyOrder.Qty);

          Match.Qty -= TradedQty;
          BuyOrder.Qty -= TradedQty;

          if (BuyOrder.Qty == 0) {
            BuyBucket[BestPrice].pop_front();
          }

          if (BuyBucket[BestPrice].empty()) {
            BuyBucket.erase(BestPrice);
            BuyPrices.pop();
          }
        }
      }
    }
  }

  auto end = std::chrono::steady_clock::now();

  auto ns =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

  return (ns / 1'000'000.0);
};