
#include <_types/_uint64_t.h>
#include <external/httplib.h>
#include <order_book.h>
#include <sys/_types/_int64_t.h>

#include <algorithm>
#include <cstddef>
#include <external/json.hpp>
#include <iostream>
#include <optional>
#include <queue>
#include <string>
#include <vector>
using json = nlohmann::json;

std::priority_queue<Order, std::vector<Order>, Compare> Order_Book() {
  // this Is Fetching the Bulk Orders
  httplib::Client cli("http://localhost:3000");
  json jsonArray;
  if (auto res = cli.Get("/")) {
    jsonArray = json::parse(res->body);
  }

  // Parsing and All

  // For Later Use If There is Any For these Normalisers
  // int Normaliser = 1000;
  // int QtyNormaliser = 100;

  std::priority_queue<Order, std::vector<Order>, Compare> OrdersPQ;

  for (auto& J : jsonArray) {
    Order o;
    o.id = J.at("id").get<std::string>();

    if (J.at("price").is_null()) {
      o.price = std::nullopt;
    } else {
      double PriceRaw = J.at("price").get<double>();
      o.price = static_cast<int64_t>(PriceRaw / 0.1);
    }

    double QtyRaw = J.at("qty").get<double>();
    o.Qty = static_cast<int64_t>(QtyRaw / 0.1);
    o.side = J.at("side").get<std::string>();
    o.tif = J.at("tif").get<std::string>();
    o.timestamp = J.at("timestamp").get<int64_t>();
    o.type = J.at("type").get<std::string>();

    OrdersPQ.push(std::move(o));
  }

  

  // Helper Function

  // std::cout << "Parsing Done I think";

  // for (const auto& Os : Orders) {
  //   std::cout << "id" << Os.id << std::endl;
  //   if (Os.price) {
  //     std::cout << "Price" << *Os.price << std::endl;
  //   } else {
  //     std::cout << "Price" << "null" << std::endl;
  //   }
  //   std::cout << "Qty" << Os.Qty << std::endl;
  //   std::cout << "side" << Os.side << std::endl;
  //   std::cout << "tif" << Os.tif << std::endl;
  //   std::cout << "timestamp" << Os.timestamp << std::endl;
  //   std::cout << "type" << Os.type << std::endl;
  // }

  return OrdersPQ;
}