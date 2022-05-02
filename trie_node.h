#ifndef ARCHIVER_TRIE_NODE_H
#define ARCHIVER_TRIE_NODE_H

#include "node.h"

class TrieNode : public Node {
public:
    TrieNode() = default;
    TrieNode(const Node& node, size_t l = 0, size_t r = 0, bool is_term = true) : Node(node), l_(l), r_(r), is_term_(is_term) {}

public:
    bool IsTerm() const;
    size_t GetL() const;
    size_t GetR() const;

private:
    bool is_term_;
    size_t l_;
    size_t r_;
};


#endif //ARCHIVER_TRIE_NODE_H
