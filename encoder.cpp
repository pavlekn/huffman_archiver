#include "encoder.h"
#include "heap.h"
#include "node.h"
#include "trie.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdexcept>

Encoder::Encoder(const std::string& archive_name, const std::vector<std::string>& file_names) {
    this->archive_name_ = archive_name;
    this->file_names_ = file_names;
}

bool Cmp(const std::pair<uint16_t, std::string>& a, const std::pair<uint16_t, std::string>& b) {
    return a.second.size() < b.second.size() || (a.second.size() == b.second.size() && a.first < b.first);
}

void Encoder::Put1bit(bool bit) {
    size_t free_bytes_left = (8 - this->cnt_written_ % 8) % 8;
    int pos = static_cast<int>(free_bytes_left) - 1;
    size_t n = this->buf_.size();
    if (free_bytes_left) {
        this->buf_[n - 1] |= (bit << pos);
    } else {
        //std::cout << " ";
        this->buf_.push_back(bit << 7);
    }
    //std::cout << bit;
    ++this->cnt_written_;
}

void Encoder::Put9bits(uint16_t val) {
    for (size_t i = 0; i < 9; ++i) {
        this->Put1bit(static_cast<bool>(val & (1 << i)));
    }
}

void Encoder::PutCode(const std::string& code) {
    for (char bit : code) {
        this->Put1bit(bit - '0');
    }
}

void Encoder::EncodeAndWriteInBuffer() {
    for (size_t cur_file_ind = 0; cur_file_ind < this->file_names_.size(); ++cur_file_ind) {
        const std::string file_name = this->file_names_[cur_file_ind];
        std::ifstream f(file_name, std::ios::binary);
        if (!f.is_open()) {
            throw std::runtime_error("No such file");
        }
        char c;
        const size_t char_cnt = 259;
        const size_t filename_end = 256;
        const size_t one_more_file = 257;
        const size_t archive_end = 258;
        std::vector<size_t> cnt(char_cnt);
        std::string file;
        while (f.get(c)) {
            ++cnt[static_cast<uint16_t>(c)];
            file.push_back(c);
        }
        for (char ch : file_name) {
            ++cnt[static_cast<uint16_t>(ch)];
        }
        ++cnt[filename_end];
        ++cnt[one_more_file];
        ++cnt[archive_end];
        Heap<Node> q;
        Trie t;
        size_t ind = 0;
        for (size_t i = 0; i < char_cnt; ++i) {
            if (cnt[i]) {
                Node node = Node(ind++, cnt[i], static_cast<uint16_t>(i));
                t.Add(node);
                q.Push(node);
            }
        }
        while (q.Size() > 1) {
            Node a = q.Pop();
            Node b = q.Pop();
            Node node = Node(ind++, a.GetCnt() + b.GetCnt());
            t.Add(TrieNode(node, a.GetInd(), b.GetInd(), false));
            q.Push(node);
        }
        std::vector<std::pair<uint16_t, std::string>> codebook;
        t.Go(codebook, t.Size() - 1);
        std::sort(codebook.begin(), codebook.end(), Cmp);
        size_t len = codebook[0].second.size();
        while (!codebook[0].second.empty()) {
            codebook[0].second.pop_back();
        }
        for (size_t i = 0; i < len; ++i) {
            codebook[0].second.push_back('0');
        }
        for (size_t i = 1; i < codebook.size(); ++i) {
            size_t add = codebook[i].second.size() - codebook[i - 1].second.size();
            std::string cur = codebook[i - 1].second;
            while (cur.back() == '1') {
                cur.pop_back();
                ++add;
            }
            cur.pop_back();
            cur.push_back('1');
            for (size_t j = 0; j < add; ++j) {
                cur.push_back('0');
            }
            codebook[i].second = cur;
        }
        std::vector<std::string> num(char_cnt);
        for (const std::pair<uint16_t, std::string>& el : codebook) {
            num[el.first] = el.second;
        }
        size_t cnt_used_chars = codebook.size();
        size_t max_code_size = codebook[cnt_used_chars - 1].second.size();
        std::vector<uint16_t> cnt_with_len(max_code_size + 1);
        this->Put9bits(cnt_used_chars);
        for (size_t i = 0; i < cnt_used_chars; ++i) {
            ++cnt_with_len[codebook[i].second.size()];
            std::cout << codebook[i].first << '\n';
            this->Put9bits(codebook[i].first);
        }
        for (size_t i = 1; i <= max_code_size; ++i) {
            this->Put9bits(cnt_with_len[i]);
        }
        for (char ch : file_name) {
            this->PutCode(num[static_cast<uint16_t>(ch)]);
        }
        this->PutCode(num[filename_end]);
        for (char ch : file) {
            this->PutCode(num[static_cast<uint16_t>(ch)]);
        }
        if (cur_file_ind == this->file_names_.size() - 1) {
            this->PutCode(num[archive_end]);
        } else {
            this->PutCode(num[one_more_file]);
        }
        f.close();
    }
}

void Encoder::WriteInArchive() {
    std::ofstream archive(this->archive_name_, std::ios::binary);
    if (!archive.is_open()) {
        throw std::runtime_error("Could not open the archive");
    }
    archive.clear();
    for (uint8_t byte : this->buf_) {
        archive.put(static_cast<char>(byte));
    }
    archive.close();
}

void Encoder::Encode() {
    this->EncodeAndWriteInBuffer();
    this->WriteInArchive();
}