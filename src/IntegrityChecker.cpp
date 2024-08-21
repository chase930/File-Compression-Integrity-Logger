#include "IntegrityChecker.h"
#include <fstream>
#include <zlib.h>
#include <cstdint>
#include <vector>
#include <algorithm>
#include "Logger.h"
namespace Utils {
namespace {
    constexpr size_t HEADER_SIZE = 16;
    constexpr char MAGIC[4] = {'M','Z','P','1'};
}
int IntegrityChecker::verifyFile(const std::string &inputPath) {
    std::ifstream file(inputPath, std::ios::binary);
    if (!file) {
        Logger::log("Failed to open file");
        return -1;
    }
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    if (size < HEADER_SIZE) {
        Logger::log("File too small");
        return -1;
    }
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(static_cast<size_t>(size));
    file.read(buffer.data(), size);
    if (!std::equal(MAGIC, MAGIC + 4, buffer.begin())) {
        Logger::log("Invalid file format");
        return -1;
    }
    uint64_t origSize = 0;
    for (size_t i = 0; i < 8; ++i) {
        origSize |= static_cast<uint64_t>(static_cast<unsigned char>(buffer[4 + i])) << (8 * i);
    }
    uint32_t storedCrc = 0;
    for (size_t i = 0; i < 4; ++i) {
        storedCrc |= static_cast<uint32_t>(static_cast<unsigned char>(buffer[12 + i])) << (8 * i);
    }
    std::vector<char> compData(buffer.begin() + HEADER_SIZE, buffer.end());
    std::vector<char> decompData(origSize);
    uLongf destLen = origSize;
    int res = uncompress(reinterpret_cast<Bytef*>(decompData.data()), &destLen, reinterpret_cast<const Bytef*>(compData.data()), compData.size());
    if (res != Z_OK) {
        Logger::log("Decompression failed");
        return res;
    }
    uint32_t crc = crc32(0, reinterpret_cast<const Bytef*>(decompData.data()), decompData.size());
    if (crc != storedCrc) {
        Logger::log("Integrity check failed");
        return -2;
    }
    Logger::log("Integrity check passed");
    return 0;
}
}
