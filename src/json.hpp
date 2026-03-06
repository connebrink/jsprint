#pragma once

#include <string>
#include <variant>
#include <vector>
#include <map>

namespace util::json {
  using namespace std;
  struct JSonNode {
    string name;
    bool isObject;
    bool isArray;
    bool isValue;
    bool isNull;
    enum VType {String, Number, Boolean} valueType;
    variant<map<string, JSonNode>, vector<JSonNode>, double, bool, string> value;
    JSonNode operator[] (const char * nName)  {
      return get<map<string, JSonNode>>(value)[nName];
    }
    JSonNode operator[] (int index) {
      return get<vector<JSonNode>>(value)[index];
    }
    operator string() const {
      if (isValue)
	return get<string>(value);
      return "";
    }
    operator double() const {
      if (isValue)
	return get<double>(value);
      return 0.0;
    }
    operator bool() const {
      if (isValue)
	return get<bool>(value);
      return false;
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
    JSonValidateInfo validate(const string& inJsonStr) const;
  public:
    map<string, JSonNode> parseIn(const string& inJsonStr,
				  JSonValidateInfo* validateInfo=nullptr) const;
  };
}
