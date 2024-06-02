#include "IntegrityChecker.h"
#include <fstream>
#include <zlib.h>
#include <cstdint>
#include <vector>
#include "Logger.h"
namespace Utils {
int IntegrityChecker::verifyFile(const std::string &inputPath) {
    std::ifstream file(inputPath, std::ios::binary);
    if(!file) {
        Logger::log("Failed to open file");
        return -1;
    }
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    if(size < 16) {
        Logger::log("File too small");
        return -1;
    }
    std::vector<char> buffer(size);
    if(!file.read(buffer.data(), size)) {
        Logger::log("Failed to read file");
        return -1;
    }
    if(buffer[0] != 'M' || buffer[1] != 'Z' || buffer[2] != 'P' || buffer[3] != '1') {
        Logger::log("Invalid file format");
        return -1;
    }
    uint64_t origSize = 0;
    for(int i = 0; i < 8; i++) {
        origSize |= (static_cast<uint64_t>(static_cast<unsigned char>(buffer[4+i])) << (8*i));
    }
    uint32_t storedCrc = 0;
    for(int i = 0; i < 4; i++) {
        storedCrc |= (static_cast<uint32_t>(static_cast<unsigned char>(buffer[12+i])) << (8*i));
    }
    std::vector<char> compData(buffer.begin() + 16, buffer.end());
    std::vector<char> decompData(origSize);
    uLongf destLen = origSize;
    int res = uncompress(reinterpret_cast<Bytef*>(decompData.data()), &destLen, reinterpret_cast<const Bytef*>(compData.data()), compData.size());
    if(res != Z_OK) {
        Logger::log("Decompression failed");
        return res;
    }
    uint32_t crc = crc32(0, reinterpret_cast<const Bytef*>(decompData.data()), decompData.size());
    if(crc != storedCrc) {
        Logger::log("Integrity check failed");
        return -2;
    }
    Logger::log("Integrity check passed");
    return 0;
}
}
