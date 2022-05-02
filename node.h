#ifndef ARCHIVER_NODE_H
#define ARCHIVER_NODE_H

#include <cstdint>

class Node {
public:
    Node () = default;
    Node (size_t ind, size_t cnt, uint16_t ch = 0) : ind_(ind), cnt_(cnt), ch_(ch) {
    }

public:
    size_t GetCnt() const;
    size_t GetInd() const;
    uint16_t GetCh() const;

public:
    bool operator<(const Node& b) const;

protected:
    uint16_t ch_;
    size_t cnt_;
    size_t ind_;
};

#endif //ARCHIVER_NODE_H
