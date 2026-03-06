
#include <iostream>

#include "json.hpp"

int main(void)
{

    std::string ert = {"{ \"ert2\":{ \"obj3\": \"1234\",  \"obj4\": false}, \"test\": \"hi\", \"testA\": 34234, \"testB\": false, \"testC\": null, \"testG\": 1.4343, \"ert\":{ \"obj1\": \"1234\",  \"obj2\": false} }"};

  util::json::JSon json;

  json.parseIn(ert);

  std::cout << std::endl << ert << std::endl;





  return 0;
}
