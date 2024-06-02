#ifndef INTEGRITYCHECKER_H
#define INTEGRITYCHECKER_H
#include <string>
namespace Utils {
class IntegrityChecker {
public:
    static int verifyFile(const std::string &inputPath);
};
}
#endif
