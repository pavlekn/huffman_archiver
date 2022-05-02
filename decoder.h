#ifndef ARCHIVER_DECODER_H
#define ARCHIVER_DECODER_H

#include <string>
#include <vector>

class Decoder {
public:
    Decoder() = default;
    explicit Decoder(const std::string& archive_name);
    void Decode();

private:
    std::string archive_name_;
    std::vector<uint8_t> buf_;
    size_t pos_ = 0;

private:
    bool Read1Bit();
    uint16_t Read9Bits();
    std::string ReadFileName(const std::vector<std::pair<uint16_t, std::string>>& codebook);
    std::string ReadFileContent(const std::vector<std::pair<uint16_t, std::string>>& codebook, bool& is_end);
    static void WriteInFile(const std::string& file_name, const std::string& content);
};

#endif //ARCHIVER_DECODER_H
