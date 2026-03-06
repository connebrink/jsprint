
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

    // auto is_number= [](const std::string& s)
    // {
    //   return !s.empty() && std::find_if(s.begin(), 
    // 					s.end(), [](unsigned char c) {
    // 					  return !std::isdigit(c); }) == s.end();
    // };
    
    bool valName   = false;
    bool isInStr   = false;
    //bool isInObj   = false;
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
	//	    cout << "l:" << level << endl;
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
	  jsonNode.isArray = false;
	  jsonNode.isObject= false;
	  jsonNode.isNull= false;
	  jsonNode.isValue = false;
	  
	  if ( (cjsValue[0] == '{')  && (cjsValue[cjsValue.length()-1] == '}') ) {
	    jsonNode.isObject = true;
	  }
	  else if ( (cjsValue[0] == '[')  && (cjsValue[cjsValue.length()-1] == ']') ) {
	    jsonNode.isArray = true;
	  }
	  else {
	    //cout << isStrValue << endl;
	    if (isStrValue) {
	      jsonNode.isValue = true;
	      JSonNodeValue value;
	      value.type = JSonNodeValue::VType::String;
	      value.value = cjsValue; 
              jsonNode.value = value;
	      isStrValue = false;
	    }
	    else {
	      if (cjsValue == "null")
		jsonNode.isNull = true;
	      else {
		JSonNodeValue value;
		jsonNode.isValue = true;
		if (cjsValue == "false" || cjsValue == "true") {
		  value.type = JSonNodeValue::VType::Boolean;
		  value.value = cjsValue == "true";
                } else  {
		  //	  cout << cjsValue << endl;
		  value.type = JSonNodeValue::VType::Number;
		  value.value = stod(cjsValue);
                }
                jsonNode.value = value;
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
	  //isInObj = false;
          if (level >= 2) {
            map<string, JSonNode> ertbb;
            // cout << "e: " << cjsName << " : " << cjsValue << endl;
            parse(cjsValue + "}", ertbb);
          }
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

      // for (auto vS : result) {
      // 	cout << vS.first << endl;
      // }
      
    }
    

    return result;
  }
}
