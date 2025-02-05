#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <unordered_set>
#include "Timer.hpp" 
#include "sequentialProcessing.hpp"
#include "multiProcessing.hpp"


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

    std::unordered_set<std::string> unique_words; //counter
    std::vector<std::thread> workers;
    size_t num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 2;
    std::cout << "Number of threads = " << num_threads << std::endl;

    std::vector<std::unordered_set<std::string>> local_maps(num_threads);//a local map of unique words per worker

    {
        Timer t{};

        for (size_t i = 0; i < num_threads; ++i) {
            workers.emplace_back(count_unique_words_worker, std::ref(local_maps[i]));
        }

        multi_file_processing(file);

        for (auto& worker : workers) {
            worker.join();
        }

        for (const auto& local_map : local_maps) {
            unique_words.insert(local_map.begin(), local_map.end());
        }

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

