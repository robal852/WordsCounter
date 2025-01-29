#include <fstream>
#include "Timer.hpp"
#include "sequentialProcessing.hpp"


int main(int argc, char* argv[]) {
    if (argc != 2){
        std::cerr << "Error: You must provide exactly one file name as an argument.\n";
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Error: Could not open file '" << argv[1] << "'.\n";
        return 1;
    }

    {
        Timer t{};
        sequential_file_processing(file);
    }

    return 0;
}

