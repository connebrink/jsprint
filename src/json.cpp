
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

  JSonNode JSon::parse(const auto &jsonStr) const {
    string cjsName;
    string cjsValue;

    bool isInStr{false};
    bool isInObject{false};
    bool isInArray{false};
    bool isStrValue{false};
    bool isSubLevel{false};

    bool valName{false};

    JSonNode result;
    result.name = ".";

    vector<JSonNode> jArray;
    map<string, JSonNode> jObject;

    for (const auto &jC : jsonStr) {
      if (jC == ' ' && !isInStr)
	continue;
      if (jC == '"' && isInStr && !isSubLevel) {
	isInStr = false;
	continue;
      } else if (jC == '"' && !isInStr && !isSubLevel) {
	isInStr = true;
	valName = cjsName.length() == 0;
	if (!valName)
	  isStrValue = true;
	continue;
      }
      if (jC == '{' && !isInStr) {
        if (!isInObject) {
	  isInObject = true;
          continue;
        }
	else
	  isSubLevel = true;
      }
      if (jC == '[' && !isInStr) {
	isInArray = true;
	jArray.clear();
	continue;
      }
      if (jC == ':' && !isInStr && cjsName != "" && !isSubLevel) {
	if (valName) {
	  valName = false;
	  continue;
	}
      }
      if ((jC == ',' || jC == '}' || jC == ']') && !isInStr && !valName && cjsValue.length() > 0) {
	if (isInArray) {
          if (!isSubLevel) {
	    if ( (cjsValue[0] == '{')  && (cjsValue[cjsValue.length()-1] == '}') ) {
	      jArray.push_back(parse(cjsValue));
	    }
	    if ( (cjsValue[0] == '[')  && (cjsValue[cjsValue.length()-1] == ']') ) {
	      jArray.push_back(parse(cjsValue));
	    }
	    else {
	      JSonNode jsonArrNode;
	      if (isStrValue) {
		jsonArrNode.isValue = true;
		jsonArrNode.valueType = JSonNode::VType::String;
		jsonArrNode.value = cjsValue; 
		isStrValue = false;
	      }
	      else {
		if (cjsValue == "null")
		  jsonArrNode.isNull = true;
		else {
		  jsonArrNode.isValue = true;
		  if (cjsValue == "false" || cjsValue == "true") {
		    jsonArrNode.valueType = JSonNode::VType::Boolean;
		    jsonArrNode.value = cjsValue == "true";
		  } else {
		    jsonArrNode.valueType = JSonNode::VType::Number;
		    jsonArrNode.value = stod(cjsValue);
		  }
		}
	      }
	      jArray.push_back(jsonArrNode);
	    }
	    cjsValue = "";
          }
          if (jC == '}') {
            if (isSubLevel)
	      isSubLevel = false;
	    else isInObject = false;
          }
	  if (jC == ']') {
	    isInArray = false;
	    result.isArray = true;
	    result.value=jArray;
	    cjsName = "";
	    cjsValue = "";
	    continue;
	  }
	}
	else if (isInObject && !isSubLevel) {
	  if ( (cjsValue[0] == '{')  && (cjsValue[cjsValue.length()-1] == '}') ) {
	    jObject[cjsName] = parse(cjsValue);
	  }
	  else {
	    JSonNode jsonNode;
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
		} else {
		  jsonNode.valueType = JSonNode::VType::Number;
		  jsonNode.value = stod(cjsValue);
		}
	      }
	    }
	    jObject[cjsName] =jsonNode;
	  }
	  //	  cout << cjsName << " : " << cjsValue << endl;
	  
	  cjsName = "";
	  cjsValue = "";
	}
  	if (jC == ',' && !isSubLevel)
	  continue;
        else if (jC == '}') {
          if (isSubLevel)
            isSubLevel = false;
          else
            isInObject = false;

          result.isObject = true;
          result.value = jObject;
        }
      }
      if (valName)
        cjsName += jC;
      else
        cjsValue += jC;
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
