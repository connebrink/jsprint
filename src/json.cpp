
#include "json.hpp"

#include <iostream>

namespace util::json {

  map<string, JSonNode> JSon::parseIn(const string &inStr) const {
    map<string, JSonNode> result;

    if (inStr == "" || inStr.length() == 0)
      return result;

    struct validateInfo {
      int oBOpen;
      int oBClose;
      int sBOpen;
      int sBClose;
      bool hasInvalidChars;
    };

    auto validate = [](const auto& jsonStr) {
      validateInfo result = {0,0,0,0,false};
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

    auto parse = [](const auto &jsonStr, const auto &jsonName, auto &result) {
      bool valName   = false;
      bool isInStr   = false;
      //bool isInObj   = false;
      bool isInArray = false;
      
      string cjsName;
      string cjsValue;
      
      if (jsonName == "") {

	int level = 0;
	
        for (const auto &jC : jsonStr) {
          if (jC == ' ')
            continue;
          if (jC == '"' && isInStr && level < 2) {
            isInStr = false;
            continue;
          } else if (jC == '"' && !isInStr && level < 2 ) {
            isInStr = true;
	    valName = cjsName.length() == 0;
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

          if ((jC == ',' || jC == '}' || jC == ']') && !isInStr) {

            if (level < 2) {
              JSonNode ert;
              result[cjsName] = ert;
              cout << cjsName << " : " << cjsValue << endl;
              cjsName = "";
              cjsValue = "";
            }

            if (jC == ',' && level < 2)
              continue;
            else if (jC == '}') {
              //isInObj = false;
	      level--;
            } else if (jC == ']')
              isInArray = false;
          }

          if (valName)
	    cjsName += jC;
	  else
	    cjsValue += jC;

        }
      }
    };

    auto validateInfo = validate(inStr);
    if (validateInfo.oBOpen == validateInfo.oBClose &&
	validateInfo.sBOpen == validateInfo.sBClose) {

      parse(inStr, "", result);

      // for (auto vS : result) {
      // 	cout << vS.first << endl;
      // }
      
    }
    

    return result;
  }
}
