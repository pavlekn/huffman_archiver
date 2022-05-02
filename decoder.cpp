#include "decoder.h"

#include <fstream>
#include <unordered_map>

Decoder::Decoder(const std::string & archive_name) {
    this->archive_name_ = archive_name;
}

bool Decoder::Read1Bit() {
    bool ans = this->buf_[this->pos_ / 8] & (1 << (7 - (this->pos_ % 8)));
    ++this->pos_;
    return ans;
}

uint16_t Decoder::Read9Bits() {
    uint16_t ans = 0;
    for (size_t i = 0; i < 9; ++i) {
        ans |= (Read1Bit() << i);
    }
    return ans;
}

std::string Decoder::ReadFileName(const std::vector<std::pair<uint16_t, std::string>>& codebook) {
    std::string file_name;
    std::unordered_map<std::string, uint16_t> num;
    const uint16_t filename_end = 256;
    for (const std::pair<uint16_t, std::string>& el : codebook) {
        num[el.second] = el.first;
    }
    std::string cur_prefix_code;
    uint16_t cur_char = 0;
    while (cur_char != filename_end) {
        while (num.find(cur_prefix_code) == num.end()) {
            cur_prefix_code.push_back('0' + Read1Bit());
        }
        cur_char = num[cur_prefix_code];
        cur_prefix_code = "";
        file_name.push_back(cur_char);
    }
    file_name.pop_back();
    return file_name;
}

void Decoder::WriteInFile(const std::string& file_name, const std::string& content) {
    std::ofstream file(file_name, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open the file");
    }
    file.clear();
    for (char byte : content) {
        file.put(byte);
    }
    file.close();
}

std::string Decoder::ReadFileContent(const std::vector<std::pair<uint16_t, std::string>>& codebook, bool& is_end) {
    std::string file_content;
    std::unordered_map<std::string, uint16_t> num;
    const uint16_t one_more_file = 257;
    const uint16_t archive_end = 258;
    for (const std::pair<uint16_t, std::string>& el : codebook) {
        num[el.second] = el.first;
    }
    std::string cur_prefix_code;
    uint16_t cur_char = 0;
    while (cur_char != one_more_file && cur_char != archive_end) {
        while (num.find(cur_prefix_code) == num.end()) {
            cur_prefix_code.push_back('0' + Read1Bit());
        }
        cur_char = num[cur_prefix_code];
        cur_prefix_code = "";
        file_content.push_back(cur_char);
        if (cur_char == archive_end) {
            is_end = true;
        }
    }
    file_content.pop_back();
    return file_content;
}

void Decoder::Decode() {
    std::ifstream archiver(this->archive_name_, std::ios::binary);
    if (!archiver.is_open()) {
        throw std::runtime_error("Archiver not found");
    }
    char c;
    while (archiver.get(c)) {
        this->buf_.push_back(static_cast<uint8_t>(c));
    }
    archiver.close();
    bool is_end = false;
    while (!is_end) {
        uint16_t cnt_used_chars = Read9Bits();
        std::vector<std::pair<uint16_t, std::string>> codebook(cnt_used_chars);
        for (size_t i = 0; i < cnt_used_chars; ++i) {
            uint16_t num = Read9Bits();
            codebook[i].first = num;
        }
        uint16_t sum_cnt = 0;
        std::vector<uint16_t> cnt_with_len = {0};
        while (sum_cnt < cnt_used_chars) {
            uint16_t cur_cnt = Read9Bits();
            cnt_with_len.push_back(cur_cnt);
            sum_cnt += cur_cnt;
        }
        std::string cur_code;
        size_t ind = 0;
        for (size_t i = 0; i < cnt_with_len.size(); ++i) {
            while (cnt_with_len[i]) {
                codebook[ind].second = cur_code;
                --cnt_with_len[i];
                ++ind;
                size_t its = 0;
                while (!cur_code.empty() && cur_code.back() != '0') {
                    cur_code.pop_back();
                    ++its;
                }
                if (cur_code.empty()) {
                    continue;
                }
                cur_code.pop_back();
                cur_code.push_back('1');
                for (size_t j = 0; j < its; ++j) {
                    cur_code.push_back('0');
                }
            }
            cur_code.push_back('0');
        }
        std::string file_name = ReadFileName(codebook);
        std::string content = ReadFileContent(codebook, is_end);
        WriteInFile(file_name, content);
    }
}