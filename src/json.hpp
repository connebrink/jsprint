#pragma once

#include <string>
#include <variant>
#include <vector>
#include <map>

namespace util::json {
  using namespace std;
  struct JSonNodeValue {
    enum VType {String, Number, Boolean} type;
    variant<double, bool, string> value;
  };
  struct JSonNode {
    bool isObject;
    bool isArray;
    bool isValue;
    bool isNull;
    variant<map<string, JSonNode>, vector<JSonNode>, JSonNodeValue> value;
    JSonNode operator[] (const string& nodeName) {
      return get<map<string, JSonNode>>(value)[nodeName];
    }
    JSonNode operator[] (int index) {
      return get<vector<JSonNode>>(value)[index];
    }
    JSonNodeValue val() {
      return get<JSonNodeValue>(value);
    }
  };
  struct JSonValidateInfo {
    int oBOpen;
    int oBClose;
    int sBOpen;
    int sBClose;
    bool hasInvalidChars;
  };
  class JSon
  {
  public:
    JSon(){};
  private:
    map<string, JSonNode> parse(const auto& jsonStr, auto& result) const;
  public:
    JSonValidateInfo validate(const string& inJsonStr) const;
  public:
    map<string, JSonNode> parseIn(const string& inJsonStr) const;
  };
}
