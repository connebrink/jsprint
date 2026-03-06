
#include <map>
#include <iostream>

#include "json.hpp"

int main(void)
{
  std::string ert = {"{\"testA\": \"hi\", \"testB\": 342,\"testO\":{\"name\":\"hello\",\"nachname\":\"world\",\"isPrivate\": true}, \"testC\": false, \"testD\": null, \"testE\": \"null\", \"testF\": true}"};
  std::cout <<  ert << std::endl << std::endl;
  util::json::JSon json;
  auto rsData = json.parseIn(ert);
  for (const auto& node : rsData) {
    if (node.second.isValue) {
      std::string vVal;
      if (node.second.valueType == util::json::JSonNode::VType::String)
        vVal = static_cast<std::string>(node.second);
      if (node.second.valueType == util::json::JSonNode::VType::Number)
        vVal = std::to_string(static_cast<double>(node.second));
      if (node.second.valueType == util::json::JSonNode::VType::Boolean)
        vVal = (static_cast<bool>(node.second)) ? "true" : "false";
      std::cout << "NodeName : " << node.first << " : " << vVal << std::endl;
    }
    else if (node.second.isObject) {
      std::cout << "NodeName : " << node.first << " : " << std::endl;
      auto oVal =  std::get<std::map<std::string, util::json::JSonNode>>(node.second.value);
      for (const auto& nn : oVal) {
        if (nn.second.isValue) {
	  std::string vVal;

	  if (nn.second.valueType == util::json::JSonNode::VType::String)
	    vVal = static_cast<std::string>(nn.second);
	  if (nn.second.valueType == util::json::JSonNode::VType::Number)
	    vVal = std::to_string(static_cast<double>(nn.second));
	  if (nn.second.valueType == util::json::JSonNode::VType::Boolean)
	    vVal = (static_cast<bool>(nn.second)) ? "true" : "false";

          std::cout << "NodeName : " << node.first << " : "  << nn.first << " : " << vVal
                    << std::endl;
        }
      }
    }
  }

  auto test = rsData["testO"];
  if (test.isObject) {
    std::cout << "isObject" << std::endl;
    auto vtest = test["name"];
    if (vtest.isValue) {
       std::cout << "isValue" << std::endl;
    }
    
  }

auto btest = static_cast<std::string>(rsData["testO"]["name"]);
std::cout << btest << std::endl;
  return 0;
}


