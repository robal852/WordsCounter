// Version with MULTITHREADING
#pragma once

#include "definitions.hpp"

void count_unique_words_worker(std::unordered_set<std::string>&);
void multi_file_processing(std::ifstream&);
