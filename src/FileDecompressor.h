#ifndef FILEDECOMPRESSOR_H
#define FILEDECOMPRESSOR_H
#include <string>
#include <vector>
namespace CompressorModule {
class FileDecompressor {
public:
    int decompressFile(const std::string &inputPath, const std::string &outputPath);
private:
    std::vector<char> loadFile(const std::string &path);
    bool saveFile(const std::string &path, const std::vector<char> &data);
};
}
#endif
