#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <bits/stdc++.h>
#include <filesystem>
#include "makeLexicon.h"

namespace fsys = std::filesystem;
using namespace rapidjson;

void load_lexicon();
int make_things(std::string& input_dir);

int main()
{
    std::string input_dir = ".\\sample\\";

    // Ensure directory exists
    if (!fsys::exists(input_dir) || !fsys::is_directory(input_dir))
    {
        std::cerr << "Error: Input directory does not exist → " << input_dir << "\n";
        return 1;
    }

    // Load existing lexicon
    load_lexicon();
    load_parsed();

    int processed = make_things(input_dir);


    if (processed > 0)
    {
        std::cout << "\nCompleted Successfully!\n";
        std::cout << "Files processed: " << processed << "\n";
        std::cout << "Lexicon size: " << lexicon.size() << "\n";
        std::cout << "Next available ID: " << current_id + 1 << "\n";
        std::cout << "Output saved in:\n";
        std::cout << "  - " << LEXICON_FILE << "\n";
    }

    return 0;
}

