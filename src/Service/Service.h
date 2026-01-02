#pragma once

#include <string>

class Service final {
public:
  Service();
  Service(Service &) = delete;
  void readTextFromFile(std::string path, std::string &result);
};
