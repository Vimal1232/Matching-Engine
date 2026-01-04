#include <matcher.h>
#include <order_book.h>
#include <sys/_types/_int64_t.h>

#include <deque>
#include <external/json.hpp>
#include <iostream>
#include <optional>
#include <ostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

void matcher() {
  std::priority_queue<Order, std::vector<Order>, Compare> PQ = Order_Book();
  std::unordered_map<int64_t, std::deque<Order>> SellBucket;
  std::unordered_map<int64_t, std::deque<Order>> BuyBucket;

  int HowManyMatched;
  int MarketOrderCancelled;

  while (!PQ.empty()) {
    Order Match = PQ.top();
    PQ.pop();

    std::optional<int64_t> RawPrice = Match.price;
    std::string Side = Match.side;

    if (RawPrice != std::nullopt) {
      if (Side == "BUY") {
        int64_t Price = RawPrice.value();
        if (SellBucket.find(Price) == SellBucket.end()) {
          BuyBucket[Price].push_back(Match);
        } else {
          Order MatchedOrder = SellBucket[Price].back();
          SellBucket[Price].pop_back();
          HowManyMatched++;
        }
      } else {
        int64_t Price = RawPrice.value();
        if (BuyBucket.find(Price) == BuyBucket.end()) {
          SellBucket[Price].push_back(Match);
        } else {
          Order MatchedOrder = BuyBucket[Price].back();
          BuyBucket[Price].pop_back();
          HowManyMatched++;
        }
      }
    } else {
      if (Side == "Buy") {
        if (SellBucket.empty()) {
          MarketOrderCancelled++;
        } else{
          
        }
      }
    }
  }
};