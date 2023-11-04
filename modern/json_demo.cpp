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
       "colors": ["blue", "red"],
       "int": 100,
       "float": 123.456
    }
  )";

  json j = json::parse(data);
  const json& v = j["colors"];
  std::cout << "colors: " << v << std::endl;
  //  use json path read json
  json result = jsonpath::json_query(j,"$.colors[*]");
  std::cout << pretty_print(result) << std::endl;
  std::cout << "result value type: " << result.type() << std::endl;
  std::cout << result[0] << std::endl;

  json app_field = jsonpath::json_query(j, "$.application");
  std::cout << "app_field type: " << app_field.type() << ", size: " << app_field.size() << std::endl;
  jsonpath::json_replace(j, "$.colors", [](const std::string& path, json& colors) {
    if (colors.is_array()) {
      auto it = colors.array_range().begin();
      it++;
      colors.insert(it, "green");
      colors.insert(colors.array_range().end(), 10);
    }
    std::cout << "replace callback colors: " << colors << std::endl;
  });
  std::cout << "after replace: " << j << std::endl;

  jsonpath::json_replace(j, "$.int", [](const std::string& path, json& value) {
    if (!value.is_number()) {
      return;
    }
    if (value.is_int64()) {
      auto v = value.as_integer<int64_t>();
      value = v + 1;
    }
  });
  std::cout << "after number incr: " << j["int"] << std::endl;
  return 0;
}
