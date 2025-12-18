#include <bits/stdc++.h>
#include "barrels.h"

std::map<int, std::map<int, std::string>> inverted_index;
HashTable* barrel_inverted_index;

//create inverted index from forward index
void build_inverted_index(std::string& forward, std::string& inverted)
{
    int wordID = 0, docID = 0, rank = 0, size = 0;
    std::ifstream fwdfile(forward);
    std::ofstream invfile(inverted);
    if (!fwdfile.is_open())
    {
        std::cerr << "Error: Cannot open " << forward << "\n";
        return;
    }
    if (!invfile.is_open())
    {
        std::cerr << "Error: Cannot open " << inverted << "\n";
        return;
    }
    while (!fwdfile.eof())
    {
        //in forward index, first is docID, second is wordID, next are rank and size
        fwdfile >> docID >> wordID >> rank >> size;
        //in inverted index, first is wordID, second is docID, next are rank and size
        invfile << wordID << " " << docID << " " << rank << " " << size << "\n";
    }
    std::cout << "Saved inverted index to file:\n";
    std::cout << " - " << inverted << "\n";
    fwdfile.close();
    invfile.close();
}

void load_barrels(std::string& inverted)
{
    barrel_inverted_index = new HashTable(1000);
    int wordID, docID, rank, size;
    std::ifstream invfile(inverted);
    Word* w;

    if (!invfile.is_open())
    {
        std::cerr << "Error: Cannot open " << inverted << "\n";
        return;
    }
    
    while (!invfile.eof())
    {
        invfile >> wordID >> docID >> rank >> size;

        w = barrel_inverted_index->get_word(wordID);
        if (w == nullptr) w = barrel_inverted_index->insert(wordID);

        w->insert_doc(docID, (float)rank / (float)(size*size));
    }
    invfile.close();
}
