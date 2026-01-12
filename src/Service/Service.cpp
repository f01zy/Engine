#include <fstream>
#include <iostream>

#include "Service.h"

Service::Service() {};

void Service::readTextFromFile(std::string path, std::string &result) {
  std::ifstream file(path);
  std::string line;

  if (!file.is_open()) {
    std::cout << "Faled to open file\n";
  }

  while (std::getline(file, line)) {
    result += line += "\n";
  }
}

int Service::getRelativePathStartsIndex(std::string path) {
  for (int i = path.size() - 1; i >= 0; i--) {
    if (path[i] == '/') {
      return i;
    }
    if (path[i] == '\\') {
      return i + 1;
    }
  }
  return 0;
}
