
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
  cout << " JSonPrint Options: " << endl;
  cout << "  '-h'          ->  This help" << endl;
  cout << "  '-j:<file>'   ->  Jsonfile, to process" << endl;
  cout << "  '-t:<theme>'  ->  Theme, to use for pretty print" << endl;
  cout << "  '-s:<style>'  ->  Style, to use for pretty print" << endl;
  cout << "  '-lt'         ->  List themes" << endl;
  cout << "  '-ls'         ->  List styles" << endl;
  cout << "  '-c'          ->  Show version" << endl;
  return 0;
}

int showThemes() {return MSG_I_SUCCEEDED;}
int showStyles() {return MSG_I_SUCCEEDED;}


void loopThrough(const util::json::JSonNode& jNode) {
  if (jNode.isObject) {
    auto oValues = get<map<string, util::json::JSonNode>>(jNode.value);
    cout << "{" << endl;
    for (const auto& node : oValues) {
      cout << "  \"" << node.first << "\": ";
      if (node.second.isValue) {
	std::string vVal;
        if (node.second.valueType == util::json::JSonNode::VType::String) {
	  vVal += "\""; 
          vVal += static_cast<std::string>(node.second);
	  vVal += "\"";
        }
        if (node.second.valueType == util::json::JSonNode::VType::Number)
	  vVal = std::to_string(static_cast<double>(node.second));
	if (node.second.valueType == util::json::JSonNode::VType::Boolean)
	  vVal = (static_cast<bool>(node.second)) ? "true" : "false";
	cout << vVal;
	if (&node != &*oValues.rbegin()) 
	  cout << ",";
	cout << endl;
      }
    }
    cout << "}" << endl;
  }  
}

int showParse(const RunInfos& rInfos) {
  util::json::JSon json;
  auto rsData = json.parseIn(rInfos.rawJsonStr);
  loopThrough(rsData);
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
