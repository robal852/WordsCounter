// Version with NO MULTITHREADING

#include "sequentialProcessing.hpp"

void sequential_file_processing(std::ifstream& file) {
    std::unordered_set<std::string> unique_words;
    std::string leftover;
    std::vector<char> buffer(BLOCK_SIZE);

    while (file.read(buffer.data(), buffer.size()) || file.gcount() > 0) {
        size_t bytes_read = file.gcount();
        std::string processed_data = leftover + std::string(buffer.data(), bytes_read);
        leftover.clear();

        // Find the last space in the input to avoid splitting a word
        size_t last_space = processed_data.find_last_of(WORD_SEPARATOR);
        if (last_space != std::string::npos) {
            leftover = processed_data.substr(last_space + 1); // Store the last incomplete word for the next iteration
            processed_data = processed_data.substr(0, last_space); // Only complete words
        }
        else {
            leftover += processed_data; // When loaded a part of a single word
        }

        std::string word;
        for (char c : processed_data) {
            if (c != WORD_SEPARATOR) {
                word += c;
            }
            else {
                if (!word.empty()) {
                    unique_words.insert(word);
                    word.clear();
                }
            }
        }
        if (!word.empty()) {
            unique_words.insert(word);
        }
    }

    // Case: no separator found in the last iteration
    if (!leftover.empty()) {
        unique_words.insert(leftover);
    }

    std::cout << "Number of unique words: " << unique_words.size() << std::endl;
}
