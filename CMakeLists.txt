cmake_minimum_required(VERSION 3.0)
project(archiver)

set(CMAKE_CXX_STANDARD 17)

add_executable(archiver main.cpp encoder.h decoder.h encoder.cpp decoder.cpp heap.h node.cpp node.h trie.cpp trie.h trie_node.cpp trie_node.h)
