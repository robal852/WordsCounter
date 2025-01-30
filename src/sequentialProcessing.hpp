// Version with NO MULTITHREADING
#pragma once

#include <iostream>
#include <fstream>
#include <unordered_set>
#include <vector>
#include "definitions.hpp"

std::string extract_part_with_complete_words(const std::string&, std::string&);
void count_unique_words(const std::string&, std::unordered_set<std::string>&);
void sequential_file_processing(std::ifstream& file);
