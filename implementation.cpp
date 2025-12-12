#include <bits/stdc++.h>
#include <filesystem>
#include "Lexicon_Forward.h"
#include "inverted_index.h"
#include "algo.h"

namespace fsys = std::filesystem;
using namespace rapidjson;

void rank(std::vector<std::vector<int>>& hitlist)
{
    int n = hitlist.size(), j = 0, i = 0;
    quickSort(hitlist, 0, n-1);
    j = 0;
    while (j < n)
    {
        while (hitlist[i].size() == hitlist[j].size()) i++;
        quickSort(hitlist, j, i-1, 0);
        j = i;
    }
    for (i = 0; i < n; i++) std::cout << hitlist[i][0]<< "\n";
}

int single_search(std::string& word)
{
    clean_token(word);
    if (word == "") return -1;
    int wordID = lexicon[word];
    std::cout << "word id " << wordID << std::endl;
    std::vector<std::vector<int>> hitlist = barrel_inverted_index->get_word(wordID)->hitlist;
    rank(hitlist);
    return hitlist.size();
}

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
        std::cout << "Lexicon size: " << current_wordID - 1 << "\n";
        std::cout << "Next available ID: " << current_wordID + 1 << "\n";
        std::cout << "Output saved in:\n";
        std::cout << "  - " << lexicon_file << "\n";
        std::cout << "  - " << forward_index_file << "\n";
    }

    build_inverted_index();
    save_inverted_index();
    load_barrels();

    std::string query = "something";
    int n = single_search(query);
    std::cout << "size " << n << std::endl;
    return 0;
}

