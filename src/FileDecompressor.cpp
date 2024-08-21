#include "FileDecompressor.h"
#include <fstream>
#include <cstdlib>
#include <zlib.h>
#include <cstdint>
#include <algorithm>
namespace CompressorModule {
namespace {
    constexpr size_t HEADER_SIZE = 16;
    constexpr char MAGIC[4] = {'M','Z','P','1'};
}
int FileDecompressor::decompressFile(const std::string &inputPath, const std::string &outputPath) {
    auto fileData = loadFile(inputPath);
    if (fileData.size() < HEADER_SIZE) return -1;
    if (!std::equal(MAGIC, MAGIC + 4, fileData.begin())) return -1;
    uint64_t origSize = 0;
    for (size_t i = 0; i < 8; ++i) {
        origSize |= static_cast<uint64_t>(static_cast<unsigned char>(fileData[4 + i])) << (8 * i);
    }
    uint32_t storedCrc = 0;
    for (size_t i = 0; i < 4; ++i) {
        storedCrc |= static_cast<uint32_t>(static_cast<unsigned char>(fileData[12 + i])) << (8 * i);
    }
    std::vector<char> compData(fileData.begin() + HEADER_SIZE, fileData.end());
    std::vector<char> decompData(origSize);
    uLongf destLen = origSize;
    int res = uncompress(reinterpret_cast<Bytef*>(decompData.data()), &destLen, reinterpret_cast<const Bytef*>(compData.data()), compData.size());
    if (res != Z_OK) return res;
    uint32_t crc = crc32(0, reinterpret_cast<const Bytef*>(decompData.data()), decompData.size());
    if (crc != storedCrc) return -2;
    if (!saveFile(outputPath, decompData)) return -1;
    return 0;
}
std::vector<char> FileDecompressor::loadFile(const std::string &path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) return {};
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    if (size <= 0) return {};
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(static_cast<size_t>(size));
    file.read(buffer.data(), size);
    return buffer;
}
bool FileDecompressor::saveFile(const std::string &path, const std::vector<char> &data) {
    std::ofstream file(path, std::ios::binary);
    if (!file) return false;
    file.write(data.data(), data.size());
    return file.good();
}
}
