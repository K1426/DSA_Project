#include <bits/stdc++.h>
#include <filesystem>
#include "Lexicon_Forward.h"

namespace fsys = std::filesystem;
using namespace rapidjson;

int main()
{
    std::string input_dir = ".\\sample\\";

    // Ensure directory exists
    if (!fsys::exists(input_dir) || !fsys::is_directory(input_dir))
    {
        std::cerr << "Error: Input directory does not exist → " << input_dir << "\n";
        return 1;
    }

    int processed = make_things(input_dir);

    if (processed > 0)
    {
        std::cout << "\nCompleted Successfully!\n";
        std::cout << "Files processed: " << processed << "\n";
        std::cout << "Lexicon size: " << lexicon.size() << "\n";
        std::cout << "Next available ID: " << current_wordID + 1 << "\n";
        std::cout << "Output saved in:\n";
        std::cout << "  - " << lexicon_file << "\n";
        std::cout << "  - " << forward_index_file << "\n";
    }

    return 0;
}

