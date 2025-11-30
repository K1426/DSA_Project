#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <bits/stdc++.h>
#include <filesystem>
#include <unordered_set>

namespace fsys = std::filesystem;
using namespace rapidjson;

// Initialization of global vars
const std::string LEXICON_FILE = "lexicon.txt", forward_index_file = "forward_index.txt";
std::ofstream lexfile, indexfile;
std::unordered_map<std::string, int> lexicon;
std::unordered_set<std::string> parsed_docs;
std::unordered_map<int, std::vector<int>> hits;
int current_id = 0;

void save_fwd_index(std::string& docID)
{
    if (!indexfile.is_open())
    {
        std::cerr << "Error: forward_index.txt file not open\n";
        return;
    }
    if (parsed_docs.insert(docID).second)
    {
        for (auto& [wordID, hit] : hits)
        {
            indexfile << docID << " " << wordID;
            for (int pos : hit) indexfile << " " << pos;
            indexfile << "\n";
        }
        parsed_docs.insert(docID);
    }
}

void load_lexicon()
{
    std::ifstream infile;

    // Load lexicon.txt if it exists
    infile.open(LEXICON_FILE);
    if (infile.is_open())
    {
        std::string word;
        while (infile >> current_id >> word) lexicon[word] = current_id;
        infile.close();
    }

    std::cout << "Loaded " << lexicon.size() << " existing lexicon entries. current_id=" << current_id << "\n";
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
    static const std::unordered_set<std::string> STOPWORDS =
    {
        "the","and","for","with","that","are","was","were","this","from","not",
        "have","has","had","but","can","may","into","its","between","our","their",
        "which","more","also","been","than","all","some","one","two","most","such"
    };
    if (STOPWORDS.find(token) != STOPWORDS.end()) token = "";
}

//enter word in lexicon
int enter_in_lexicon(std::string& word)
{
    if (word == "") return -1;
    if (lexicon.find(word) == lexicon.end())
    {
        lexicon[word] = ++current_id;
        if (lexfile.is_open()) lexfile << current_id << " " << word << '\n';
        else std::cout << "Warning: lexicon output stream not open. Word not written: " << word << "\n";
    }
    return current_id;
}

// Add words from content to lexicon
void make_lexicon_and_fwd_index(std::string& docID, std::string& content)
{
    std::istringstream ss(content);
    std::string word;
    int wordID = 0, pos = 0;
    while (ss >> word)
    {
        clean_token(word);
        if (word != "")
        {
            wordID = enter_in_lexicon(word);
            hits[wordID].push_back(++pos);
        }
    }
    save_fwd_index(docID);
    hits.clear();
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
    if (!fp)
    {
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

//check for already parsed files
void load_parsed()
{
    std::ifstream infile;
    //check if forward index exists
    if (!fsys::exists(forward_index_file) || fsys::is_empty(forward_index_file)) return;

    // Load parsed.txt if it exists
    infile.open("parsed.txt");
    
    if (infile.is_open())
    {
        std::string docID;
        while (infile >> docID) parsed_docs.insert(docID);
        infile.close();
    }
}

//save record of parsed docs
void save_parsed()
{
    std::ofstream parsed("parsed.txt");
    if (!parsed.is_open())
    {
        std::cerr << "Error: Cannot open parsed.txt\n";
        return;
    }
    for (std::string docID : parsed_docs) parsed << docID << "\n";
    parsed.close();
}

int make_things(std::string& input_dir)
{
    lexfile.open(LEXICON_FILE, std::ios::app);
    indexfile.open(forward_index_file, std::ios::app);
    std::string content = "", docID = "";
    int processed = 0;

    // List files in the directory
    std::vector<std::string> files = list_files(input_dir);
    std::cout << "Found " << files.size() << " files in directory: " << input_dir << "\n";

    // Open lexicon.txt in append mode
    if (!lexfile.is_open())
    {
        std::cerr << "Error: Cannot open lexicon file: " << LEXICON_FILE << "\n";
        return 0;
    }
    if (!indexfile.is_open())
    {
        std::cerr << "Error: Cannot open forward_index.txt file\n";
        return 0;
    }

    // Process files to build lexicon
    for (std::string fpath : files)
    {
        docID = fpath.substr(fpath.rfind('\\') + 1);
        content = fetch_json_data(fpath.c_str());
        if (content == "") continue;

        make_lexicon_and_fwd_index(docID, content);
        processed++;

        if (processed % 1000 == 0)
        {
            std::cout << "Processed " << processed << std::setw(7) << std::left 
                << " files" << " | Current lexicon size: "
                << lexicon.size() << "\n";
        }
    }

    save_parsed();

    //close file
    lexfile.close();
    indexfile.close();
    return processed;
}
