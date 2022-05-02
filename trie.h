#ifndef ARCHIVER_TRIE_H
#define ARCHIVER_TRIE_H

#include "trie_node.h"

#include <vector>
#include <string>

class Trie {
public:
    Trie() = default;

public:
    void Add(const TrieNode& x);
    void Go(std::vector<std::pair<uint16_t, std::string>>& codebook, size_t ind, const std::string& code = "");
    size_t Size();

private:
    std::vector<TrieNode> nodes_;
};

#endif //ARCHIVER_TRIE_H
