
#include <map>
#include <iostream>

#include "json.hpp"

int main(void)
{
  // std::string ert = {"{ \"ert2\":{ \"obj3\": \"1234\",  \"obj4\": false}, \"test\": \"hi\", \"testA\": 34234, \"testB\": false, \"testC\": null, \"testG\": 1.4343, \"ert\":{ \"obj1\": \"1234\",  \"obj2\": { \"obj30\": \"hallo\",  \"obj40\": false}} }"};
  std::string ert = {"{\"testA\": \"hi\", \"testB\": 342,\"testO\":{\"name\":\"hello\",\"nachname\":\"world\",\"isPrivate\": true}, \"testC\": false, \"testD\": null, \"testE\": \"null\", \"testF\": true}"};

  std::cout <<  ert << std::endl << std::endl;
  
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
    }
    else if (node.second.isObject) {
      std::cout << "NodeName : " << node.first << " : " << std::endl;
      auto oVal =  std::get<std::map<std::string, util::json::JSonNode>>(node.second.value);
      for (const auto& nn : oVal) {
        if (nn.second.isValue) {
	  auto rVal =  std::get<util::json::JSonNodeValue>(nn.second.value);
          std::string nVal;
          if (rVal.type == util::json::JSonNodeValue::String)
            nVal = std::get<std::string>(rVal.value);
          if (rVal.type == util::json::JSonNodeValue::Number)
            nVal = std::to_string(std::get<double>(rVal.value));
          if (rVal.type == util::json::JSonNodeValue::Boolean)
            nVal = std::get<bool>(rVal.value) ? "true" : "false";
          std::cout << "NodeName : " << node.first << " : "  << nn.first << " : " << nVal
                    << std::endl;
        }
      }
    }
  }
  
  return 0;
}


