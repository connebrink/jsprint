
#include <iostream>
#include <vector>

#include "json.hpp"

namespace util::json {
  JSonValidateInfo JSon::validate(const string& jsonStr) const {
    JSonValidateInfo result = {0,0,0,0,false};
    bool isInStrVal = false;
    for (const auto& jC : jsonStr) {
      if (jC == ' ')
	continue;
      if (jC == '"' && isInStrVal) {
	isInStrVal = false;
	continue;
      } else if (jC == '"' && !isInStrVal)
	isInStrVal = true;
      if (jC == '{' && !isInStrVal)
	result.oBOpen++;
      if (jC == '}' && !isInStrVal)
	result.oBClose++;
      if (jC == '[' && !isInStrVal)
	result.sBOpen++;
      if (jC == ']' && !isInStrVal)
	result.sBClose++;
    }
    return result;
  };

  JSonNode JSon::parse(const auto &jsonStr, bool isRoot) const {
    JSonNode result;
    if (isRoot)
      result.name = ".";
    vector<JSonNode> jArray;
    map<string, JSonNode> jObject;
    bool isInStr{false};
    bool isStrValue{false};
    bool isObj{false};
    bool isArr{false};
    bool valName{false};
    int subOLevel{-1};
    string nName;
    string nValue;
    auto skipChars = [](const auto& jsChar) {
      if (jsChar == ' ')
	return true;
      return false;
    };
    for (const auto &jC : jsonStr) {
      if (skipChars(jC) && !isInStr) continue;
      if (jC == '{' && !isInStr) {
	isObj = true;
	subOLevel++;
	if (!subOLevel)
	  continue;
      }
      if (jC == '"') {
	isInStr = !isInStr;
        if (!subOLevel) {
	  if (isObj)
	    valName = nName.length() == 0;
          continue;
        }
      }
      if (jC == '[' && !isInStr) {
	isArr = true;
	subOLevel++;
	valName= false;
        if (!subOLevel) {
          continue;
        }
      }
      if (!subOLevel && jC == ':') {
        valName = false;
        continue;
      }
      if (!subOLevel && (jC == ',' || jC == '}' || jC == ']') ) {
	//cout << subOLevel  << endl;
	cout << nName << " : " << nValue << endl;
	JSonNode jsonNodeValue;
        if ((nValue[0] == '{') && (nValue[nValue.length() - 1] == '}')) {
	  jsonNodeValue = parse(nValue, false);
        }
        else if ((nValue[0] == '[') && (nValue[nValue.length() - 1] == ']')) {
          jsonNodeValue = parse(nValue, false);
        } else {
          if (isArr){
	    jsonNodeValue.isArray = true;
	    jArray.push_back(jsonNodeValue);
	  }
          if (isObj) {
            jObject[nName] = jsonNodeValue;
          }
          if (isStrValue) {
	    isStrValue = false;
	  }
        }
	nName  = "";
	nValue = "";
	if (isObj)
	  valName=true;
	if (jC == ',')
	  continue;
      }
      if (valName)
        nName += jC;
      else
        nValue += jC;
      if ((jC == '}' || jC == ']') && !isInStr) {
        subOLevel--;
      }
    }
    return result;
  }

  JSonNode JSon::parseIn(const string &jsonStr, JSonValidateInfo* validateInfo) const {
    if (validateInfo) {
      auto validateInfo = validate(jsonStr);
      if (validateInfo.oBOpen == validateInfo.oBClose &&
          validateInfo.sBOpen == validateInfo.sBClose) {
        return parse(jsonStr);
      }
    }
    return parse(jsonStr);
  }
}
