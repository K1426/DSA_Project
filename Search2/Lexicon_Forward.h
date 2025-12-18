#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <bits/stdc++.h>
#include <filesystem>
#include "auto.h"

namespace fsys = std::filesystem;
using namespace rapidjson;

// Initialization of global vars
const std::string lexicon_file = "lexicon.txt", forward_index_file = "forward_index.txt", parsedfile = "parsed_files.txt";
Trie lexicon;
std::vector<std::string> titles, paths;
int current_wordID = 0;

//allll the functions
void load_lexicon();
std::vector<std::string> list_files(std::string& dirPath);
std::string parse_json(const char* fname);
void parse_content(std::string& content, std::unordered_map<int, std::pair<int, int>>& ranks);
void clean_token(std::string& token);
int enter_in_lexicon(std::string& word, std::ofstream& lexfile);
void save_to_fwd_index(int docID, std::ofstream& indexfile, std::unordered_map<int, std::pair<int, int>>& ranks);

//load lexicon if exists
void load_lexicon()
{
    std::ifstream infile(lexicon_file);
    if (infile.is_open())
    {
        std::string word;
        while (infile >> current_wordID >> word) lexicon.insert(word, current_wordID);
        infile.close();
    }
    std::cout << "Loaded " << lexicon.getSize() << " existing lexicon entries. current_wordID=" << current_wordID << "\n";
}

//new file paths for regular files in directory
int list_files(std::string& dirPath, std::vector<std::string>& files)
{
    std::unordered_set<std::string> parsed_files;
    int docID = 0;
    std::string info = "";
    std::ifstream get_docs(parsedfile);
    if (get_docs.is_open())
    {
        while (!get_docs.eof())
        {
            get_docs >> docID >> info;
            paths.push_back(info);
            parsed_files.insert(info);
            getline(get_docs, info);
            titles.push_back(info);
        }
        get_docs.close();
    }
    
    try
    {
        for (const auto& file : fsys::directory_iterator(dirPath))
        {
            info = file.path().string();
            if (fsys::is_regular_file(file.path()) && parsed_files.find(info) == parsed_files.end())
                files.push_back(info);
        }
    }
    catch (fsys::filesystem_error& e)
    {
        std::cerr << "Filesystem error while listing files: " << e.what() << "\n";
        exit(0);
    }

    return docID;
}

//fetch content from txt
std::string parse_txt(std::string& path)
{
    std::string content = "", temp = "";
    std::ifstream infile(path);
    if (!infile.is_open())
    {
        std::cerr << path << " not open\n";
        return "";
    }
    while(getline(infile, temp)) content += temp + "\n";
    infile.close();
    return content;
}

//fetch text content from CORD-19 JSON file
std::string parse_json(const char* fname)
{
    static char readBuffer[65536];
    std::string text = "";

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
            text = md["title"].GetString();
        }
    }

    // Abstract (array of objects with "text")
    if (doc.HasMember("abstract") && doc["abstract"].IsArray())
    {
        if (text != "") text += "\n\n";
        text += "Abstract: ";
        for (const auto& para : doc["abstract"].GetArray())
        {
            if (para.IsObject() && para.HasMember("text") && para["text"].IsString())
            {
                text += para["text"].GetString();
            }
        }
    }

    // Body text (array)
    if (doc.HasMember("body_text") && doc["body_text"].IsArray())
    {
        if (text != "") text += "\n\n";
        text += "Main Text: ";
        for (const auto& para : doc["body_text"].GetArray())
        {
            if (para.IsObject() && para.HasMember("text") && para["text"].IsString())
            {
                text += para["text"].GetString();
                text += "\n\n";
            }
        }
    }

    return text;
}

//parse content for lexicon and forward index
void parse_content(std::string& content, std::ofstream& lexfile, std::unordered_map<int, std::pair<int, int>>& ranks)
{
    std::istringstream ss(content);
    std::string word;
    int wordID = 0, pos = 0;
    while (ss >> word)
    {
        clean_token(word);
        if (word != "")
        {
            ++pos;
            wordID = enter_in_lexicon(word, lexfile);
            ranks[wordID].first+=pos;
            ranks[wordID].second++;
        }
    }
    
}

//clean word
void clean_token(std::string& token)
{
    int i = 0;
    if (token == "") return;

    //only alphanumeric
    token.erase(std::remove_if(token.begin(), token.end(), [](char c){return !std::isalnum(c) && c != '-';}), token.end ());
    
    //no short words
    if (token.length() < 3) {token = ""; return;}

    //to lowercase
    transform(token.begin(), token.end(), token.begin(), [](char c) {return tolower(c);});

    //at least two alphabetic chars
    for (char c : token) if ((int)c >= 97 && (int)c <= 122) i++;
    if (i < 2) {token = ""; return;}
}

//enter word in lexicon
int enter_in_lexicon(std::string& word, std::ofstream& lexfile)
{
    if (lexicon.search(word) == 0)
    {
        current_wordID++;
        lexicon.insert(word, current_wordID);
        if (lexfile.is_open()) lexfile << current_wordID << " " << word << '\n';
        else std::cout << "Warning: lexicon output stream not open. Word not written: " << word << "\n";
    }
    return lexicon.search(word);
}

//save document ranks in forward index
void save_to_fwd_index(int docID, std::ofstream& indexfile, std::unordered_map<int, std::pair<int, int>>& ranks)
{
    if (!indexfile.is_open())
    {
        std::cerr << "Error: forward_index.txt file not open\n";
        exit(0);
    }
    for (const auto& [wordID, rank] : ranks)
    {
        indexfile << docID << " " << wordID << " " << rank.first << " " << rank.second << "\n";
    }
}

int make_things(std::string& input_dir)
{
    int docID = 0, processed = 0;
    std::string content = "", title = "";
    std::vector<std::string> files;
    std::ofstream parsed_files;
    std::unordered_map<int, std::pair<int, int>> ranks;
    
    load_lexicon();
    docID = list_files(input_dir, files);
    std::cout << "Found " << files.size() << " new files in directory: " << input_dir << "\n";

    std::ofstream lexfile(lexicon_file, std::ios::app);
    std::ofstream indexfile(forward_index_file, std::ios::app);
    parsed_files.open(parsedfile, std::ios::app);
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
    for (auto& path : files)
    {
        ++docID;
        if (path.substr(path.rfind('.')) == ".json") content = parse_json(path.c_str());
        else content = parse_txt(path);
        if (content == "") continue;
        title = content.substr(0, content.find('\n'));
        titles.push_back(title);
        paths.push_back(path);
        parse_content(content, lexfile, ranks);
        save_to_fwd_index(docID, indexfile, ranks);
        ranks.clear();
        parsed_files << docID << " " << path << " " << title << "\n";
        processed++;
        if (processed % 1000 == 0)
        {
            std::cout << "Processed " << processed << std::setw(7) << std::left 
                << " files" << " | Current lexicon size: "
                << lexicon.getSize() << "\n";
        }
    }
    
    lexfile.close();
    indexfile.close();
    parsed_files.close();
    return processed;
}
