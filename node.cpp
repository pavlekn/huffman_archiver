#include "node.h"

size_t Node::GetCnt() const {
    return cnt_;
}

uint16_t Node::GetCh() const {
    return ch_;
}

size_t Node::GetInd() const {
    return ind_;
}

bool Node::operator<(const Node& b) const {
    return (*this).cnt_ < b.cnt_ || ((*this).cnt_ == b.cnt_ && (*this).ch_ < b.ch_);
}