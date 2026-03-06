
#include <iostream>

#include "json.hpp"

int main(void)
{

    // std::string ert = {"{ \"ert2\":{ \"obj3\": \"1234\",  \"obj4\": false}, \"test\": \"hi\", \"testA\": 34234, \"testB\": false, \"testC\": null, \"testG\": 1.4343, \"ert\":{ \"obj1\": \"1234\",  \"obj2\": { \"obj30\": \"hallo\",  \"obj40\": false}} }"};

    std::string ert = {"{\"testA\": \"hi\", \"testB\": 342, \"testC\": false, \"testD\": null, \"testE\": \"null\", \"testF\": true}"};

    util::json::JSon json;

    auto rsData = json.parseIn(ert);

for (const auto& node : rsData) {
  if (node.second.isValue) {

    auto oVal =  std::get<util::json::JSonNodeValue>(node.second.value);

    std::string vVal;
    
    if (oVal.type == util::json::JSonNodeValue::String)
      vVal = std::get<std::string>(oVal.value);
    if (oVal.type == util::json::JSonNodeValue::Number)
      vVal = std::to_string(std::get<double>(oVal.value));
    if (oVal.type == util::json::JSonNodeValue::Boolean)
      vVal = std::get<bool>(oVal.value) ? "true" : "false";
    
    std::cout << "NodeName : " << node.first << " : " << vVal << std::endl;
    //  std::cout << "NodeName : " << node.first << std::endl;
  }
  

}




    std::cout << std::endl << ert << std::endl;

    //  std::string ertA =
    //  {"{\"urls\":{\"signed\":\"http://localhost:8945/doc/d28283be\"},\"document\":{\"signedId\":\"d28283be\",\"expiredAt\":\"2026-03-06T01:48:08\"}}"};

    //  json.parseIn(ertA);

    // std::cout << std::endl << ertA << std::endl;
    return 0;
}


// "{\"urls\":{\"signed\":\"http://localhost:8945/doc/d28283be\"},\"document\":{\"signedId\":\"d28283be\",\"expiredAt\":\"2026-03-06T01:48:08\"}}"
