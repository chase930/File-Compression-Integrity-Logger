#ifndef LOGGER_H
#define LOGGER_H
#include <string>
namespace Utils {
class Logger {
public:
    static void log(const std::string &message);
};
}
#endif
