#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <bits/stdc++.h>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <unordered_set>

namespace fsys = std::filesystem;
using namespace rapidjson;

// ------------- Configuration -------------
const std::string LEXICON_FILE = "lexicon.txt";
std::unordered_map<std::string, int> word_to_id;
int current_id = 0;


void load_lexicon()
{
    std::ifstream infile;

    // Load lexicon.txt if it exists
    infile.open(LEXICON_FILE);
    if (infile.is_open())
    {
        std::string word;
        while (infile >> current_id >> word) word_to_id[word] = current_id;
        infile.close();
    }

    std::cout << "Loaded " << word_to_id.size() << " existing lexicon entries. current_id=" << current_id << "\n";
}

// Token cleaning logic - returns cleaned token or empty string if invalid
void clean_token(std::string& token)
{
    int i = 0;
    if (token == "") return;
    // Trim punctuation
    token.erase(std::remove_if(token.begin(), token.end(), [](char c){return ispunct(c) && c != '-';}), token.end ());
    
    if (token.length() < 3) {token = ""; return;}
    
    // Discard very short tokens and non alphanumeric strings
    if (!std::all_of(token.begin(), token.end(), [](char c) {return std::isalnum(c) || c == '-';})) {token = ""; return;}

    // Convert to lowercase
    transform(token.begin(), token.end(), token.begin(), [](char c) {return tolower(c);});

    // Require at least two alphabetic characters
    for (char c : token) if ((int)c >= 97 && (int)c <= 122) i++;
    if (i < 2) {token = ""; return;}
    
    // Remove common stopwords
    static const std::unordered_set<std::string> STOPWORDS = {
        "the","and","for","with","that","are","was","were","this","from","not",
        "have","has","had","but","can","may","into","its","between","our","their",
        "which","more","also","been","than","all","some","one","two","most","such"
    };
    if (STOPWORDS.find(token) != STOPWORDS.end()) token = "";
}

int enter_in_lexicon(std::string& word, std::ofstream& outfile)
{
    if (word == "") return -1;
    if (word_to_id.find(word) == word_to_id.end())
    {
        word_to_id.emplace(word, ++current_id);
        if (outfile.is_open()) outfile << current_id << " " << word << '\n';
        else std::cout << "Warning: lexicon output stream not open. Word not written: " << word << "\n";
    }
    return word_to_id[word];
}

// Returns vector of full file paths for regular files in directory (non-recursive)
std::vector<std::string> list_files(std::string& dirPath)
{
    std::vector<std::string> files;
    try
    {
        for (const auto& entry : fsys::directory_iterator(dirPath)) {
            if (fsys::is_regular_file(entry.path()))
                files.push_back(entry.path().string());
        }
    }
    catch (fsys::filesystem_error& e)
    {
        std::cerr << "Filesystem error while listing files: " << e.what() << "\n";
    }
    return files;
}

// Fetch text content from a CORD-19 JSON file
std::string fetch_json_data(const char* fname)
{
    static char readBuffer[65536];
    std::string text;

    FILE* fp = nullptr;
    fopen_s(&fp, fname, "rb");
    if (!fp) {
        std::cerr << "Warning: cannot open file " << fname << "\n";
        return "";
    }

    FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    Document doc;
    doc.ParseStream(is);
    fclose(fp);

    if (doc.HasParseError())
    {
        std::cerr << "Warning: parse error in file " << fname << "\n";
        return "";
    }

    // Title
    if (doc.HasMember("metadata") && doc["metadata"].IsObject())
    {
        const Value& md = doc["metadata"];
        if (md.HasMember("title") && md["title"].IsString())
        {
            text += md["title"].GetString();
            text += " ";
        }
    }

    // Abstract (array of objects with "text")
    if (doc.HasMember("abstract") && doc["abstract"].IsArray())
    {
        for (const auto& para : doc["abstract"].GetArray())
        {
            if (para.IsObject() && para.HasMember("text") && para["text"].IsString())
            {
                text += para["text"].GetString();
                text += " ";
            }
        }
    }

    // Body text (array)
    if (doc.HasMember("body_text") && doc["body_text"].IsArray())
    {
        for (const auto& para : doc["body_text"].GetArray())
        {
            if (para.IsObject() && para.HasMember("text") && para["text"].IsString())
            {
                text += para["text"].GetString();
                text += " ";
            }
        }
    }

    return text;
}

// Add words from content to lexicon
void make_things(const std::string& content, std::ofstream& outfile)
{
    std::istringstream ss(content);
    std::string word;
    int pos = 0, id = 0;;
    while (ss >> word)
    {
        clean_token(word);
        if (word != "") pos = enter_in_lexicon(word, outfile);
    }
}