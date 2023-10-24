//
// Created by Shiping Yao on 2023/10/24.
//

#include <jsoncons/json.hpp>
#include <jsoncons_ext/jsonpath/jsonpath.hpp>

using namespace jsoncons;

int main() {
  std::string data = R"(
    {
       "application": "hiking",
       "reputons": [
       {
           "rater": "HikingAsylum",
           "assertion": "advanced",
           "rated": "Marilyn C",
           "rating": 0.90,
           "generated": 1514862245
         }
       ],
       "colors": ["blue", "red"]
    }
  )";

  json j = json::parse(data);
  const json& v = j["colors"];
  std::cout << "colors: " << v << std::endl;
  //  use json path read json
  json result = jsonpath::json_query(j,"$.colors");
  std::cout << pretty_print(result) << std::endl;
  return 0;
}
