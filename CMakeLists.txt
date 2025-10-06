cmake_minimum_required(VERSION 4.0)
project(HuffmanTextFileCompressor)

set(CMAKE_CXX_STANDARD 20)

add_executable(HuffmanTextFileCompressor main.cpp
        Storage.cpp
        Storage.h
        StorageDriver.cpp
        Huffman.cpp
        Huffman.h
        Node.h
        HuffmanDriver.cpp)
