#include "Logger.h"
#include <iostream>
namespace Utils {
void Logger::log(const std::string &message) {
    std::cout << message << std::endl;
}
}
