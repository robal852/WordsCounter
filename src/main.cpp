#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <unordered_set>
#include "Timer.hpp" 
#include "sequentialProcessing.hpp"
#include "MultiThreadedWordCounter.hpp"


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Error: You must provide exactly one file name as an argument.\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Error: Could not open file '" << argv[1] << "'.\n";
        return 1;
    }

    std::cout << "BLOCK_SIZE = " << BLOCK_SIZE / 1024 << "KB\n";
    size_t num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 2;
    std::cout << "Number of threads = " << num_threads << std::endl;

    {
        Timer t{};
        MultiThreadedWordCounter counter(num_threads);
        counter.process_file(file);
        auto unique_words = counter.get_all_unique_words();
        std::cout << "Number of unique words: " << unique_words.size() << std::endl;
    }


    std::cout << "\nWithout multithreading:\n";
    file.clear();
    file.seekg(0);
    {
        Timer t{};
        sequential_file_processing(file);
    }

    return 0;
}

