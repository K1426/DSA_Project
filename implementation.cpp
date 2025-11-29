#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <cctype>

namespace fsys = std::filesystem;
using namespace rapidjson;
using namespace std;

unordered_map<string, int> word_to_id;

// Reuse cleaning logic
string clean_token(string token) {
    for (int i = 0; i < token.length(); i++)
        if (ispunct(token[i]) && token[i] != '-')
            token.erase(i, 1);

    if (token.length() < 3) return "";
    transform(token.begin(), token.end(), token.begin(), ::tolower);

    int alphaCount = 0;
    for (char c : token) if (isalpha(c)) alphaCount++;
    if (alphaCount < 2) return "";

    return token;
}

// Load lexicon
void load_lexicon() {
    ifstream in("lexicon.txt");
    int id; string word;
    while (in >> id >> word) {
        word_to_id[word] = id;
    }
    in.close();
    cout << "Loaded " << word_to_id.size() << " words from lexicon.\n";
}

// List JSON files
vector<string> list_files(string path) {
    vector<string> files;
    for (const auto& entry : fsys::directory_iterator(path))
        if (fsys::is_regular_file(entry.path()))
            files.push_back(entry.path().string());
    return files;
}

// Read content from JSON
string fetch_content(const char* fname) {
    static char buffer[65536];
    string text;

    FILE* fp = nullptr;
    fopen_s(&fp, fname, "rb");
    if (!fp) return "";

    FileReadStream is(fp, buffer, sizeof(buffer));
    Document doc;
    doc.ParseStream(is);
    fclose(fp);

    if (doc.HasParseError()) return "";

    if (doc.HasMember("body_text") && doc["body_text"].IsArray()) {
        for (auto& p : doc["body_text"].GetArray())
            if (p.IsObject() && p.HasMember("text"))
                text += p["text"].GetString(), text += " ";
    }
    return text;
}

int main() {
    load_lexicon();

    string input_dir = "./sample/";
    auto files = list_files(input_dir);
    ofstream forward_out("forward_index.txt");

    int doc_id = 0;
    for (string file : files) {
        unordered_map<int, vector<int>> word_positions;

        string content = fetch_content(file.c_str());
        istringstream ss(content);
        string word;
        int position = 0;

        while (ss >> word) {
            word = clean_token(word);
            if (word.empty() || word_to_id.find(word) == word_to_id.end()) continue;
            int word_id = word_to_id[word];
            word_positions[word_id].push_back(position);
            position++;
        }

        forward_out << doc_id;
        for (auto& p : word_positions) {
            forward_out << " " << p.first << ":" << p.second.size() << ":(";
            for (size_t i = 0; i < p.second.size(); i++) {
                forward_out << p.second[i];
                if (i + 1 < p.second.size()) forward_out << ",";
            }
            forward_out << ")";
        }
        forward_out << "\n";

        doc_id++;
        if (doc_id % 50 == 0)
            cout << "Processed " << doc_id << " documents\n";
    }

    forward_out.close();
    cout << "\nForward index created. Total docs: " << doc_id << endl;
    return 0;
}
