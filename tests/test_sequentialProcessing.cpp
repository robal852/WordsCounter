#include "gtest/gtest.h"
#include "../src/sequentialProcessing.hpp"
#include <fstream>

TEST(SequentialProcessingTest, extract_part_with_complete_words_test) {

    std::string leftover;
    std::string result = extract_part_with_complete_words("a b c test", leftover);

    EXPECT_EQ(result, "a b c");
    EXPECT_EQ(leftover, "test");
}

TEST(SequentialProcessingTest, count_unique_words_test) {
    
    std::unordered_set<std::string> unique_words;
    
    count_unique_words("a b a b aa bb aa bb", unique_words);
    
    EXPECT_EQ(unique_words.size(), 4);
    EXPECT_TRUE(unique_words.find("a") != unique_words.end());
    EXPECT_TRUE(unique_words.find("b") != unique_words.end());
    EXPECT_TRUE(unique_words.find("aa") != unique_words.end());
    EXPECT_TRUE(unique_words.find("bb") != unique_words.end());
}
