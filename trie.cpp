#include "trie.h"

void Trie::Add(const TrieNode& x) {
    nodes_.push_back(x);
}

void Trie::Go(std::vector<std::pair<uint16_t, std::string>>& codebook, size_t ind, const std::string& code) {
    if (nodes_[ind].IsTerm()) {
        codebook.emplace_back(nodes_[ind].GetCh(), code);
        return;
    }
    Go(codebook, nodes_[ind].GetL(), code + '0');
    Go(codebook, nodes_[ind].GetR(), code + '1');
}

size_t Trie::Size() {
    return nodes_.size();
}