#pragma once

#include <string>
#include <unordered_set>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>
#include <thread>
#include <fstream>
#include "definitions.hpp"

class MultiThreadedWordCounter {
public:
    explicit MultiThreadedWordCounter(size_t num_threads);

    ~MultiThreadedWordCounter();

    void process_file(std::ifstream& file);

    std::unordered_set<std::string> get_all_unique_words() const;

private:
    void count_unique_words_worker(std::unordered_set<std::string>& local_set);

    std::string extract_part_with_complete_words(const std::string& inputData, std::string& leftover);

    void count_unique_words(const std::string& processed_data, std::unordered_set<std::string>& unique_words);

    std::queue<std::string> processing_queue;
    mutable std::mutex mtx;
    std::condition_variable queue_cv;
    std::atomic<bool> finished_reading;
    std::vector<std::thread> workers;
    std::vector<std::unordered_set<std::string>> local_sets; //a local set of unique words per worker
    size_t num_threads;
};
