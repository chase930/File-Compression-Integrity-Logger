# FileCompressorLogger

## Overview
FileCompressorLogger is a modular C++ utility that compresses, decompresses, and verifies file integrity. It embeds metadata (magic number, original file size, and CRC32) in each compressed file and leverages zlib for high‐performance data compression.

## How I Solved the Problem
- Implemented separate modules for compression, decompression, and integrity checking, simplifying code maintenance and testing.
- Used zlib to handle the core compression and decompression routines, reducing the complexity of manual compression algorithms.
- Added a custom 16-byte header (magic number, original size, and CRC32) to each compressed file, allowing the decompressor to validate file integrity before fully extracting data.
- Built a basic logging module to provide consistent messaging for successes and failures.

## Function Overview
- **CompressorModule::FileCompressor**  
  - `int compressFile(const std::string &inputPath, const std::string &outputPath, int compressionLevel = -1)`: Loads the input file, compresses it, adds the custom header, and writes to disk.

- **CompressorModule::FileDecompressor**  
  - `int decompressFile(const std::string &inputPath, const std::string &outputPath)`: Reads the compressed file, checks the header, verifies integrity via CRC32, and decompresses the data.

- **Utils::IntegrityChecker**  
  - `static int verifyFile(const std::string &inputPath)`: Validates a compressed file without writing any output, ensuring data integrity by comparing the stored CRC32 to the recomputed value.

- **Utils::Logger**  
  - `static void log(const std::string &message)`: Provides a central point for console output, making the code more maintainable and extensible.

## How to Run (Use) the Program

1. **Build the Project**  
   - Install [CMake](https://cmake.org/) and [zlib](https://zlib.net/).
   - From the project root, create a build directory:
     ```bash
     mkdir build && cd build
     cmake ..
     make
     ```

2. **Execute the Binary**  
   - Navigate to the build directory and run:
     ```bash
     ./FileCompressorLogger <mode> [parameters]
     ```

3. **Supported Modes**  
   - **compress**  
     ```bash
     ./FileCompressorLogger compress <input_file> <output_file> [compressionLevel]
     ```
     Compresses `<input_file>` into `<output_file>`. If no `compressionLevel` is specified, zlib’s default is used.

   - **decompress**  
     ```bash
     ./FileCompressorLogger decompress <input_file> <output_file>
     ```
     Decompresses `<input_file>` into `<output_file>` and verifies file integrity using the embedded header.

   - **verify**  
     ```bash
     ./FileCompressorLogger verify <input_file>
     ```
     Performs an integrity check on `<input_file>` without writing any decompressed output.

## References
- [zlib Official Site](https://zlib.net/)  
- [CMake Documentation](https://cmake.org/documentation/)  
- [GitHub Repository (Optional)](https://github.com/yourusername/FileCompressorLogger)
