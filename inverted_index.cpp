#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <bits/stdc++.h>
#include <filesystem>
#include <unordered_map>

namespace fsys = std::filesystem;
using namespace rapidjson;

std::map<int, std::map<std::string, std::string>> inverted_index;

//create inverted index from forward index
void load_inverted_index()
{
    std::string docID, hits;
    int wordID = 0;
    std::ifstream fwdfile("forward_index.txt");

    if (!fwdfile.is_open())
    {
        std::cerr << "Error: Cannot open forward_index.txt file\n";
        return;
    }
    //in file, first is docID, second is wordID, next are hits
    while (!fwdfile.eof())
    {
        fwdfile >> docID >> wordID;
        getline(fwdfile, hits);
        //in inverted index, first is wordID, second is docID, next are hits
        inverted_index[wordID][docID] = hits;
    }
    std::cout << "Loaded inverted index from forward index\n";
}


//save inverted index to file
void save_inverted_index()
{
    std::ofstream invfile("inverted_index.txt");
    if (!invfile.is_open())
    {
        std::cerr << "Error: Cannot open inverted_index.txt file\n";
        return;
    }
    for (auto& [wordID, data]: inverted_index)
    for (auto& [docID, hits]: data)
        invfile << wordID << " " << docID << hits << "\n";
    invfile.close();
    std::cout << "Saved inverted index to file\n";
}

int main()
{
    //load and save inverted index
    load_inverted_index();
    save_inverted_index();
    return 0;
}