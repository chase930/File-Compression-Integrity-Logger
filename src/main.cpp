#include <iostream>
#include <cstdlib>
#include <string>
#include "FileCompressor.h"
#include "FileDecompressor.h"
#include "IntegrityChecker.h"
#include "Logger.h"
int main(int argc, char* argv[]) {
    if (argc < 2) {
        Utils::Logger::log("Usage: <mode> [parameters]");
        return 1;
    }
    std::string mode = argv[1];
    if (mode == "compress") {
        if (argc < 4) {
            Utils::Logger::log("Usage: compress <input> <output> [compressionLevel]");
            return 1;
        }
        std::string input = argv[2];
        std::string output = argv[3];
        int level = -1;
        if (argc > 4) level = std::atoi(argv[4]);
        CompressorModule::FileCompressor compressor;
        int res = compressor.compressFile(input, output, level);
        Utils::Logger::log(res == 0 ? "Compression successful" : "Compression failed");
        return res;
    } else if (mode == "decompress") {
        if (argc < 4) {
            Utils::Logger::log("Usage: decompress <input> <output>");
            return 1;
        }
        std::string input = argv[2];
        std::string output = argv[3];
        CompressorModule::FileDecompressor decompressor;
        int res = decompressor.decompressFile(input, output);
        Utils::Logger::log(res == 0 ? "Decompression successful" : "Decompression failed");
        return res;
    } else if (mode == "verify") {
        if (argc < 3) {
            Utils::Logger::log("Usage: verify <input>");
            return 1;
        }
        std::string input = argv[2];
        int res = Utils::IntegrityChecker::verifyFile(input);
        return res;
    }
    Utils::Logger::log("Unknown mode");
    return 1;
}
