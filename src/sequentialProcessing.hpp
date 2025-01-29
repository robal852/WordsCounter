// Version with NO MULTITHREADING
#pragma once

#include <iostream>
#include <fstream>
#include <unordered_set>
#include <vector>

#define WORD_SEPARATOR ' '
const size_t BLOCK_SIZE = 512 * 1024; // 512 KB - Block size for file reading

void sequential_file_processing(std::ifstream& file);
