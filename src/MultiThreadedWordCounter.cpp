#include "MultiThreadedWordCounter.hpp"
#include <iostream>

MultiThreadedWordCounter::MultiThreadedWordCounter(size_t num_threads)
    : finished_reading(false), num_threads(num_threads)
{
    local_sets.resize(num_threads);
}

MultiThreadedWordCounter::~MultiThreadedWordCounter() {
    for (auto& worker : workers) {
        if (worker.joinable()) {
            worker.join(); // Ensure all worker threads have been joined.
        }
    }
}

void MultiThreadedWordCounter::process_file(std::ifstream& file) {
    for (size_t i = 0; i < num_threads; ++i) {
        workers.emplace_back(&MultiThreadedWordCounter::count_unique_words_worker, this, std::ref(local_sets[i]));
    }

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

    //finish
    for (auto& worker : workers) {
        if (worker.joinable())
            worker.join();
    }
}

std::unordered_set<std::string> MultiThreadedWordCounter::get_all_unique_words() const {
    std::unordered_set<std::string> unique_words;
    for (const auto& local_set : local_sets) {
        unique_words.insert(local_set.begin(), local_set.end());
    }
    return unique_words;
}

void MultiThreadedWordCounter::count_unique_words_worker(std::unordered_set<std::string>& local_set) {
    try {
        while (true) {
            std::string data;
            {
                std::unique_lock<std::mutex> lock(mtx);
                queue_cv.wait(lock, [this] {
                    return !processing_queue.empty() || finished_reading.load();
                    });

                if (processing_queue.empty() && finished_reading.load()) {
                    return; //finished
                }

                data = std::move(processing_queue.front());
                processing_queue.pop();
            }
            count_unique_words(data, local_set);
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Exception in worker thread " << std::this_thread::get_id()
            << ": " << ex.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown exception in worker thread " << std::this_thread::get_id() << std::endl;
    }
}

std::string MultiThreadedWordCounter::extract_part_with_complete_words(const std::string& input_data, std::string& leftover) {
    std::string full_data = leftover + input_data;
    leftover.clear();

    // Find the last space in the input to avoid splitting a word
    size_t last_space = full_data.find_last_of(WORD_SEPARATOR);
    if (last_space != std::string::npos) {
        leftover = full_data.substr(last_space + 1); // Store the last incomplete word for the next iteration
        return full_data.substr(0, last_space); // Only complete words
    }

    // When loaded a part of a single word
    leftover += full_data;
    return "";
}

void MultiThreadedWordCounter::count_unique_words(const std::string& processed_data, std::unordered_set<std::string>& unique_words) {
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
