
#include "json.hpp"

#include <iostream>
#include <algorithm>

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

  map<string, JSonNode> JSon::parse(const auto &jsonStr, auto &result) const {
    
    bool valName   = false;
    bool isInStr   = false;
    bool isInArray = false;
      
    string cjsName;
    string cjsValue;
 
    int level = 0;

    bool isStrValue = false;
	
    for (const auto &jC : jsonStr) {
      if (jC == ' ')
	continue;
      if (jC == '"' && isInStr && level < 2) {
	isInStr = false;
	continue;
      } else if (jC == '"' && !isInStr && level < 2 ) {
	isInStr = true;
	valName = cjsName.length() == 0;
	if (!valName)
	  isStrValue = true;
	continue;
      }

      if (jC == '{' && !isInStr) {
	level++;
	if (level < 2)
	  continue;
      }

      if (jC == '[' && !isInArray && !isInStr) {
	isInArray = true;
	continue;
      }

      if (jC == ':' && !isInStr && cjsName != "" && level < 2) {
	if (valName) {
	  valName = false;
	  continue;
	}
      }

      if ((jC == ',' || jC == '}' || jC == ']') && !isInStr && !valName) {

	if (level < 2) {
	  JSonNode jsonNode;
	  jsonNode.name = cjsName;
	  jsonNode.isArray = false;
	  jsonNode.isObject= false;
	  jsonNode.isNull= false;
	  jsonNode.isValue = false;
	  if ( (cjsValue[0] == '{')  && (cjsValue[cjsValue.length()-1] == '}') ) {
	    jsonNode.isObject = true;
	    map<string, JSonNode> sObj;
	    jsonNode.value = parse(cjsValue, sObj);
	    jsonNode.value = sObj;
	    //cout << cjsValue << endl;
	  }
	  else if ( (cjsValue[0] == '[')  && (cjsValue[cjsValue.length()-1] == ']') ) {
	    jsonNode.isArray = true;
	  }
	  else {
	    //cout << isStrValue << endl;
	    if (isStrValue) {
	      jsonNode.isValue = true;
	      jsonNode.valueType = JSonNode::VType::String;
	      jsonNode.value = cjsValue; 
	      isStrValue = false;
	    }
	    else {
	      if (cjsValue == "null")
		jsonNode.isNull = true;
	      else {
		jsonNode.isValue = true;
		if (cjsValue == "false" || cjsValue == "true") {
		  jsonNode.valueType = JSonNode::VType::Boolean;
		  jsonNode.value = cjsValue == "true";
                } else  {
		  jsonNode.valueType = JSonNode::VType::Number;
		  jsonNode.value = stod(cjsValue);
                }
              }
            }
          }

	  //	  cout << cjsName << " : " << cjsValue << endl; 

	  result[cjsName] = jsonNode;
	  cjsName = "";
	  cjsValue = "";
        }

	if (jC == ',' && level < 2)
	  continue;
	else if (jC == '}') {
          level--;
	} else if (jC == ']')
	  isInArray = false;
      }

      if (valName)
	cjsName += jC;
      else
	cjsValue += jC;

    }
    return result;
  }

  map<string, JSonNode> JSon::parseIn(const string &jsonStr) const {
    map<string, JSonNode> result;
    auto validateInfo = validate(jsonStr);
    if (validateInfo.oBOpen == validateInfo.oBClose &&
	validateInfo.sBOpen == validateInfo.sBClose) {
      parse(jsonStr, result);
    }
    return result;
  }
}
