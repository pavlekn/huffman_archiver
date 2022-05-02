#include "trie_node.h"

bool TrieNode::IsTerm() const {
    return is_term_;
}

size_t TrieNode::GetL() const {
    return l_;
}

size_t TrieNode::GetR() const {
    return r_;
}