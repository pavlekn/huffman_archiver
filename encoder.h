#ifndef ARCHIVER_ENCODER_H
#define ARCHIVER_ENCODER_H

#include <string>
#include <vector>

class Encoder {
public:
    Encoder() = default;
    Encoder(const std::string& archive_name, const std::vector<std::string>& file_names);

public:
    void Encode();

private:
    std::string archive_name_;
    std::vector<std::string> file_names_;
    std::vector<uint8_t> buf_;
    size_t cnt_written_ = 0;

private:
    void Put9bits(uint16_t val);
    void Put1bit(bool bit);
    void PutCode(const std::string& code);
    void EncodeAndWriteInBuffer();
    void WriteInArchive();
};

#endif //ARCHIVER_ENCODER_H
