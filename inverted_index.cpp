#include <bits/stdc++.h>
#include "barrels.h"

std::map<int, std::map<int, std::string>> inverted_index;
HashTable* barrel_inverted_index;

//create inverted index from forward index
void build_inverted_index()
{
    std::string hits;
    int wordID = 0, docID = 0;
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
    inverted_index.clear();
}

void load_barrels()
{
    barrel_inverted_index = new HashTable(100);
    int wordID, docID, pos;
    std::string hit;
    std::stringstream ss;
    std::vector<int> hits;
    std::ifstream invfile("inverted_index.txt");
    Word* w;
    
    if (!invfile.is_open())
    {
        std::cerr << "Error: Cannot open inverted_index.txt file\n";
        return;
    }
    
    while (!invfile.eof())
    {
        invfile >> wordID >> docID;
        getline(invfile, hit);
        ss.str(hit);
        while (ss >> pos) hits.push_back(pos);

        w = barrel_inverted_index->get_word(wordID);
        if (w == nullptr) w = barrel_inverted_index->insert(wordID);

        w->insert_hit(docID, hits);
        hits.clear();
    }
    invfile.close();
}