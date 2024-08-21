#include "FileCompressor.h"
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
int FileCompressor::compressFile(const std::string &inputPath, const std::string &outputPath, int compressionLevel) {
    auto inputData = loadFile(inputPath);
    if (inputData.empty()) return -1;
    uint32_t crc = crc32(0, reinterpret_cast<const Bytef*>(inputData.data()), inputData.size());
    uLongf compBound = compressBound(inputData.size());
    std::vector<char> compData(compBound);
    int res = compress2(reinterpret_cast<Bytef*>(compData.data()), &compBound, reinterpret_cast<const Bytef*>(inputData.data()), inputData.size(), (compressionLevel < 0 ? Z_DEFAULT_COMPRESSION : compressionLevel));
    if (res != Z_OK) return res;
    compData.resize(compBound);
    std::vector<char> outData;
    outData.resize(HEADER_SIZE + compData.size());
    std::copy(MAGIC, MAGIC + 4, outData.begin());
    uint64_t origSize = inputData.size();
    for (size_t i = 0; i < 8; ++i) {
        outData[4 + i] = (origSize >> (8 * i)) & 0xFF;
    }
    for (size_t i = 0; i < 4; ++i) {
        outData[12 + i] = (crc >> (8 * i)) & 0xFF;
    }
    std::copy(compData.begin(), compData.end(), outData.begin() + HEADER_SIZE);
    if (!saveFile(outputPath, outData)) return -1;
    return 0;
}
std::vector<char> FileCompressor::loadFile(const std::string &path) {
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
bool FileCompressor::saveFile(const std::string &path, const std::vector<char> &data) {
    std::ofstream file(path, std::ios::binary);
    if (!file) return false;
    file.write(data.data(), data.size());
    return file.good();
}
}
