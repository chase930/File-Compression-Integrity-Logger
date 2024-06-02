#ifndef FILECOMPRESSOR_H
#define FILECOMPRESSOR_H
#include <string>
#include <vector>
namespace CompressorModule {
class FileCompressor {
public:
    int compressFile(const std::string &inputPath, const std::string &outputPath, int compressionLevel = -1);
private:
    std::vector<char> loadFile(const std::string &path);
    bool saveFile(const std::string &path, const std::vector<char> &data);
};
}
#endif
