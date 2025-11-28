#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

#include <filesystem>
#include <cstdio>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <unordered_set>

namespace fs = std::filesystem;
using namespace rapidjson;

// ------------- Configuration -------------
const std::string SAMPLE_DIR = ".\\sample\\";      // input JSON files directory
const std::string LEXICON_FILE = "lexicon.txt";
const std::string COUNT_FILE = "count.txt";

// ------------- Globals -------------
std::unordered_map<std::string, int> word_to_id;
int next_id = 0;

// ------------- Utilities -------------
void load_existing_lexicon_and_count() {
    // Load lexicon.txt if it exists
    std::ifstream lexIn(LEXICON_FILE);
    if (lexIn.is_open()) {
        int id; std::string word;
        while (lexIn >> id >> word) {
            word_to_id[word] = id;
            if (id >= next_id) next_id = id + 1;
        }
        lexIn.close();
    }

    // Load count.txt if it exists (prefer this to override next_id)
    std::ifstream countIn(COUNT_FILE);
    if (countIn.is_open()) {
        int c; if (countIn >> c) next_id = std::max(next_id, c);
        countIn.close();
    }

    std::cout << "Loaded " << word_to_id.size() << " existing lexicon entries. next_id=" << next_id << "\n";
}

void save_count() {
    std::ofstream out(COUNT_FILE, std::ios::trunc);
    if (!out) {
        std::cerr << "Error: cannot write " << COUNT_FILE << "\n";
        return;
    }
    out << next_id << "\n";
    out.close();
}

// Token cleaning logic - returns cleaned token or empty string if invalid
void clean_token(std::string& token)
{
    // Trim punctuation
    token.erase(std::remove_if(token.begin(), token.end(), [] (char c){return std::ispunct(c) && c != '-';}), token.end());
    
    // Discard very short tokens and non alphanumeric strings
    if (token.length() < 3 || !std::all_of(token.begin(), token.end(), [](char c) {return std::isalnum(c) || c == '-';})) token = "";

    // Convert to lowercase
    std::transform(token.begin(), token.end(), token.begin(), tolower);

    // Require at least two alphabetic characters
    int alphaCount = 0;
    for (char c : token) if (isalpha(static_cast<unsigned char>(c))) alphaCount++;
    if (alphaCount < 2) token = "";

    // Remove common stopwords
    static const std::unordered_set<std::string> STOPWORDS = {
        "the","and","for","with","that","are","was","were","this","from","not",
        "have","has","had","but","can","may","into","its","between","our","their",
        "which","more","also","been","than","all","some","one","two","most","such"
    };
    if (STOPWORDS.count(token)) token = "";
}

void make_entry(const std::string& word, std::ofstream& lexOut) {
    if (word == "") return;
    if (word_to_id.find(word) != word_to_id.end()) return;

    int id = next_id++;
    word_to_id.emplace(word, id);
    if (lexOut.is_open()) lexOut << id << " " << word << '\n';
    else std::cerr << "Warning: lexicon output stream not open. Word not written: " << word << "\n";
}

// Returns vector of full file paths for regular files in directory (non-recursive)
std::vector<std::string> list_files_in_directory(const std::string& dirPath) {
    std::vector<std::string> files;
    try
    {
        for (const auto& entry : fs::directory_iterator(dirPath)) {
            if (fs::is_regular_file(entry.path())) {
                files.push_back(entry.path().string());
            }
        }
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "Filesystem error while listing files: " << e.what() << "\n";
    }
    std::sort(files.begin(), files.end());
    return files;
}

// Fetch text content from a CORD-19 JSON file
std::string fetch_data_from_json(const char* fname) {
    static char readBuffer[65536];  // Fix stack warning
    std::string text;

    FILE* fp = nullptr;
    fopen_s(&fp, fname, "rb");  // Secure open
    if (!fp) {
        std::cerr << "Warning: cannot open file " << fname << "\n";
        return "";
    }

    FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    Document doc;
    doc.ParseStream(is);
    fclose(fp);

    if (doc.HasParseError()) {
        std::cerr << "Warning: parse error in file " << fname << "\n";
        return "";
    }

    // Title
    if (doc.HasMember("metadata") && doc["metadata"].IsObject()) {
        const Value& md = doc["metadata"];
        if (md.HasMember("title") && md["title"].IsString()) {
            text += md["title"].GetString();
            text += " ";
        }
    }

    // Abstract (array of objects with "text")
    if (doc.HasMember("abstract") && doc["abstract"].IsArray()) {
        for (const auto& para : doc["abstract"].GetArray()) {
            if (para.IsObject() && para.HasMember("text") && para["text"].IsString()) {
                text += para["text"].GetString();
                text += " ";
            }
        }
    }

    // Body text (array)
    if (doc.HasMember("body_text") && doc["body_text"].IsArray()) {
        for (const auto& para : doc["body_text"].GetArray()) {
            if (para.IsObject() && para.HasMember("text") && para["text"].IsString()) {
                text += para["text"].GetString();
                text += " ";
            }
        }
    }

    return text;
}

// Add words from content to lexicon
void add_lexicon_from_content(const std::string& content, std::ofstream& lexOut)
{
    std::istringstream ss(content);
    std::string raw_token;
    while (ss >> raw_token)
    {
        clean_token(raw_token);
        if (!raw_token.empty()) make_entry(raw_token, lexOut);
    }
}
int main() {
    // Replace SAMPLE_DIR with your actual path
    std::string input_dir = "D:\\DSA_Project\\DSA_Project\\sample\\";  // or your previous code's path

    // Ensure directory exists
    if (!fs::exists(input_dir) || !fs::is_directory(input_dir)) {
        std::cerr << "Error: Input directory does not exist → " << input_dir << "\n";
        return 1;
    }

    // Load existing lexicon & count
    load_existing_lexicon_and_count();

    // Open lexicon.txt in append mode
    std::ofstream lexOut(LEXICON_FILE, std::ios::app);
    if (!lexOut) {
        std::cerr << "Error: Cannot open lexicon file → " << LEXICON_FILE << "\n";
        return 1;
    }

    // List files in the directory
    auto files = list_files_in_directory(input_dir);
    std::cout << "Found " << files.size() << " files in directory: " << input_dir << "\n";

    // Process files to build lexicon
    size_t processed = 0;
    for (const auto& fpath : files) {
        std::string content = fetch_data_from_json(fpath.c_str());
        if (content.empty()) continue;

        add_lexicon_from_content(content, lexOut);
        processed++;

        if (processed % 50 == 0) {  // Save progress every 50 files
            lexOut.flush();
            save_count();
            std::cout << "Processed " << processed
                << " files | Current lexicon size: "
                << word_to_id.size() << "\n";
        }
    }

    // Final save
    lexOut.close();
    save_count();

    std::cout << "\n Completed Successfully!\n";
    std::cout << " Files processed: " << processed << "\n";
    std::cout << " Lexicon size: " << word_to_id.size() << "\n";
    std::cout << " Next available ID: " << next_id << "\n";
    std::cout << " Output saved in:\n";
    std::cout << "   - " << LEXICON_FILE << "\n";
    std::cout << "   - " << COUNT_FILE << "\n";

    return 0;
}
