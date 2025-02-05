// Version with MULTITHREADING
#include <iostream>
#include <unordered_set>
#include <string>
#include <queue>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>
#include "multiProcessing.hpp"
#include "sequentialProcessing.hpp"



std::queue<std::string> processing_queue;
std::mutex mtx;
std::condition_variable queue_cv;
std::atomic<bool> finished_reading{ false };

void count_unique_words_worker(std::unordered_set<std::string>& local_words) {
    try {
        while (true) {
            std::string data;

            {
                std::unique_lock<std::mutex> lock(mtx);
                queue_cv.wait(lock, [] { return !processing_queue.empty() || finished_reading.load(); });

                if (processing_queue.empty() && finished_reading.load()) {
                    return; //finished
                }

                data = std::move(processing_queue.front()); // get data to process
                processing_queue.pop();
            }

            //std::cout << "Thread " << std::this_thread::get_id() << " is processing data.\n";

            count_unique_words(data, local_words);
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Exception in thread " << std::this_thread::get_id() << ": " << ex.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown exception in thread " << std::this_thread::get_id() << std::endl;
    }
}

void multi_file_processing(std::ifstream& file) {
    std::string leftover;
    std::vector<char> buffer(BLOCK_SIZE);

    while (file.read(buffer.data(), buffer.size()) || file.gcount() > 0) {
        size_t bytes_read = file.gcount();
        std::string processed_data = extract_part_with_complete_words(std::string(buffer.data(), bytes_read), leftover);

        {
            std::lock_guard<std::mutex> lock(mtx);
            processing_queue.push(std::move(processed_data));
        }
        queue_cv.notify_one();
    }

    //last word
    if (!leftover.empty()) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            processing_queue.push(std::move(leftover));
        }
        queue_cv.notify_one();
    }

    finished_reading.store(true);
    queue_cv.notify_all();
}