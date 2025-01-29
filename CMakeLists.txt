cmake_minimum_required(VERSION 3.10)
project(WordsCounter)

set(CMAKE_CXX_STANDARD 17)

# Google Test
include(FetchContent)
FetchContent_Declare(
	  googletest
	    GIT_REPOSITORY https://github.com/google/googletest.git
	      GIT_TAG        release-1.11.0
      )
      FetchContent_MakeAvailable(googletest)


      set(SOURCE_FILES
	          src/main.cpp
		      src/sequentialProcessing.cpp
	      )

	      set(HEADER_FILES
		          src/Timer.hpp
			      src/sequentialProcessing.hpp
		      )

		      add_executable(wordsCounter ${SOURCE_FILES} ${HEADER_FILES})

		      # Tests

		      add_executable(test_sequentialProcessing tests/test_sequentialProcessing.cpp src/sequentialProcessing.cpp)
		      target_link_libraries(test_sequentialProcessing gtest_main)



