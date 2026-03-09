
#include <string>
#include <fstream>
#include <iostream>
#include <map>

#include "json.hpp"

const int MSG_I_SUCCEEDED {0};
const int MSG_I_ERROR     {101};

using namespace std;

struct RunInfos {
  enum class Action { SHELP, STHEMES, SSTYLES, SPARSE } action;
  enum class Style {sNormal} style;
  enum class Theme {tDark} theme;
  string rawJsonStr;
};

string readJsonStrFromFile(const string& fullFileName) {
  string result;
  ifstream file(fullFileName);
  if (file.is_open()) {
    string line;
    while (getline(file, line))
      result += line;
    file.close();
  }
  return result;
};

string readJsonStrFromStandardIn() {
  string result;
  for (string line; getline(cin, line);) 
    result += line;
  return result;
};

RunInfos loadRunInfo(int argCount, char **argValues) {
  RunInfos result;
  string jsonFileName;
  for (auto i = 0; i < argCount; ++i) {
    string tArgValue{argValues[i]};
  }
  if (jsonFileName.length() > 0)
    result.rawJsonStr = readJsonStrFromFile(jsonFileName);
  else
    result.rawJsonStr = readJsonStrFromStandardIn();
  result.action = result.rawJsonStr.empty() ? RunInfos::Action::SHELP : RunInfos::Action::SPARSE;
  return result;
}

int showHelp() {
  return 0;
}

int showThemes() {return MSG_I_SUCCEEDED;}
int showStyles() {return MSG_I_SUCCEEDED;}

int showParse(const RunInfos& rInfos) {
  util::json::JSon json;
  auto rsData = json.parseIn(rInfos.rawJsonStr);

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
    } else if (node.second.isArray) {
      
      auto oVal =  std::get<std::vector<util::json::JSonNode>>(node.second.value);
      for (const auto &nn : oVal) {

	if (nn.isValue) {
	  std::string vVal;

	  if (nn.valueType == util::json::JSonNode::VType::String)
	    vVal = static_cast<std::string>(nn);
	  if (nn.valueType == util::json::JSonNode::VType::Number)
	    vVal = std::to_string(static_cast<double>(nn));
	  if (nn.valueType == util::json::JSonNode::VType::Boolean)
	    vVal = (static_cast<bool>(nn)) ? "true" : "false";

          std::cout << "NodeName : " << node.first << " : "  << vVal
                    << std::endl;
        }

	if (nn.isObject) {
	  auto oVal =  std::get<std::map<std::string, util::json::JSonNode>>(nn.value);
	  for (const auto& tt : oVal){
	    //	    std::cout << tt.first << std::endl;

	    std::string vVal;

	    if (tt.second.valueType == util::json::JSonNode::VType::String)
	      vVal = static_cast<std::string>(tt.second);
	    if (tt.second.valueType == util::json::JSonNode::VType::Number)
	      vVal = std::to_string(static_cast<double>(tt.second));
	    if (tt.second.valueType == util::json::JSonNode::VType::Boolean)
	      vVal = (static_cast<bool>(tt.second)) ? "true" : "false";

	    std::cout << "NodeName : "  << node.first << " : " << tt.first << " : "  <<  vVal << std::endl;
	  }
	}
      }
    }
  }

  
  return MSG_I_SUCCEEDED;
}

int main(int argCount, char **argValues) {
  RunInfos rInfos = loadRunInfo(argCount, argValues);
  if (rInfos.action == RunInfos::Action::SHELP)
    return showHelp();
  if (rInfos.action == RunInfos::Action::STHEMES)
    return showThemes();
  if (rInfos.action == RunInfos::Action::SSTYLES)
    return showStyles();
  if (rInfos.action == RunInfos::Action::SPARSE)
    return showParse(rInfos);
  return MSG_I_ERROR;
}

  // std::string ert = {"{\"testA\": \"hi test\", \"testB\": "
  //                    "342,\"testO\":{\"name\":\"hello\",\"nachname\":\"world\","
  //                    "\"isPrivate\": true}, \"testC\": false, \"testD\": null, "
  //                    "\"testE\": \"null\", \"testF\": true, \"testZ\": "
  //                    "[\"a\",\"b\",{\"sO1\": 2323, \"sO2\": \"hi\"},\"c\"]}"};


//   auto test = rsData["testO"];
//   if (test.isObject) {
//     std::cout << "isObject" << std::endl;
//     auto vtest = test["name"];
//     if (vtest.isValue) {
//        std::cout << "isValue" << std::endl;
//     }
    
//   }

// auto btest = static_cast<std::string>(rsData["testO"]["name"]);
// std::cout << btest << std::endl;
//   return 0;
// }


