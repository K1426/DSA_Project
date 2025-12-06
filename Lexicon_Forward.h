#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <bits/stdc++.h>
#include <filesystem>
#include <unordered_set>

namespace fsys = std::filesystem;
using namespace rapidjson;

// Initialization of global vars
const std::string lexicon_file = "lexicon.txt", forward_index_file = "forward_index.txt";
std::ofstream lexfile, indexfile;
std::unordered_map<std::string, int> lexicon;
int current_wordID = 0;
bool is_fwd = false;

//allll the functions
void load_lexicon();
std::vector<std::string> list_files(std::string& dirPath);
std::string parse_json(const char* fname);
void parse_content(std::string& content, std::unordered_map<int, std::vector<int>>& hits);
void clean_token(std::string& token);
int enter_in_lexicon(std::string& word);
void save_to_fwd_index(int docID, std::unordered_map<int, std::vector<int>>& hits);

//load lexicon if exists
void load_lexicon()
{
    std::ifstream infile(lexicon_file);
    if (infile.is_open())
    {
        std::string word;
        while (infile >> current_wordID >> word) lexicon[word] = current_wordID;
        infile.close();
    }
    std::cout << "Loaded " << lexicon.size() << " existing lexicon entries. current_wordID=" << current_wordID << "\n";
}

//new file paths for regular files in directory
int list_files(std::string& dirPath, std::vector<std::string>& files)
{
    std::unordered_set<std::string> parsed_files;
    int doc_ID = 0;
    std::string filepath = "";
    std::ifstream get_docs("doc_id.txt");
    if (get_docs.is_open())
    {
        while (get_docs >> doc_ID >> filepath) parsed_files.insert(filepath);
        get_docs.close();
    }
    
    try
    {
        for (const auto& file : fsys::directory_iterator(dirPath))
        {
            filepath = file.path().string();
            if (fsys::is_regular_file(file.path()) && parsed_files.insert(filepath).second)
                files.push_back(filepath);
        }
    }
    catch (fsys::filesystem_error& e)
    {
        std::cerr << "Filesystem error while listing files: " << e.what() << "\n";
        exit(0);
    }

    return doc_ID;
}

//fetch text content from CORD-19 JSON file
std::string parse_json(const char* fname)
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

//parse content for lexicon and forward index
void parse_content(std::string& content, std::unordered_map<int, std::vector<int>>& hits)
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
    
}

//clean word
void clean_token(std::string& token)
{
    int i = 0;
    if (token == "") return;

    //no punctuation
    token.erase(std::remove_if(token.begin(), token.end(), [](char c){return ispunct(c) && c != '-';}), token.end ());
    
    //no short words
    if (token.length() < 3) {token = ""; return;}
    
    //only alphanumeric words
    if (!std::all_of(token.begin(), token.end(), [](char c) {return std::isalnum(c) || c == '-';})) {token = ""; return;}

    //to lowercase
    transform(token.begin(), token.end(), token.begin(), [](char c) {return tolower(c);});

    //at least two alphabetic chars
    for (char c : token) if ((int)c >= 97 && (int)c <= 122) i++;
    if (i < 2) {token = ""; return;}
    
    //remove common words
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
    if (lexicon.find(word) == lexicon.end())
    {
        lexicon[word] = ++current_wordID;
        if (lexfile.is_open()) lexfile << current_wordID << " " << word << '\n';
        else std::cout << "Warning: lexicon output stream not open. Word not written: " << word << "\n";
    }
    return lexicon[word];
}

//save document hits in forward index
void save_to_fwd_index(int docID, std::unordered_map<int, std::vector<int>>& hits)
{
    if (!indexfile.is_open())
    {
        std::cerr << "Error: forward_index.txt file not open\n";
        exit(0);
    }
    for (auto& [wordID, hit] : hits)
    {
        indexfile << docID << " " << wordID;
        for (int pos : hit) indexfile << " " << pos;
        indexfile << "\n";
    }
}

int make_things(std::string& input_dir)
{
    int docID = 0, processed = 0;
    std::string content = "";
    std::vector<std::string> files;
    std::ofstream parsed_files;
    std::unordered_map<int, std::vector<int>> hits;
    
    load_lexicon();
    docID = list_files(input_dir, files);
    std::cout << "Found " << files.size() << " new files in directory: " << input_dir << "\n";

    lexfile.open(lexicon_file, std::ios::app);
    indexfile.open(forward_index_file, std::ios::app);
    parsed_files.open("parsed_files.txt", std::ios::app);
    if (!lexfile.is_open())
    {
        std::cerr << "Error: Cannot open lexicon file: " << lexicon_file << "\n";
        return 0;
    }
    if (!indexfile.is_open())
    {
        std::cerr << "Error: Cannot open forward_index.txt file\n";
        return 0;
    }
    if (!parsed_files.is_open())
    {
        std::cerr << "Error: Cannot open parsed_files.txt file\n";
        return 0;
    }

    //process all files
    for (std::string path : files)
    {
        ++docID;
        content = parse_json(path.c_str());
        if (content == "") continue;
        parse_content(content, hits);
        save_to_fwd_index(docID, hits);
        hits.clear();
        parsed_files << docID << path << std::endl;
        processed++;
        if (processed % 1000 == 0)
        {
            std::cout << "Processed " << processed << std::setw(7) << std::left 
                << " files" << " | Current lexicon size: "
                << lexicon.size() << "\n";
        }
    }

    lexfile.close();
    indexfile.close();
    parsed_files.close();
    return processed;
}
