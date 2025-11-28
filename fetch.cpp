#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <filesystem>
#include <bits/stdc++.h>
using namespace rapidjson;
namespace fsys = std::filesystem;

std::string path = ".\\lexicon\\", idfile = "count.txt";;
std::ofstream file;
std::ifstream infile;
int id = 0;

void clean(std::string& s)
{
    int j = 0;

    transform(s.begin(), s.end(), s.begin(), tolower);
    if (s.length() < 3 || !all_of(s.begin(), s.end(), [](char c) {return isalnum(c) || c == '-';})) {s = "a"; return;}
    for (char c : s)
    {
        if (isalpha(c)) j++;
        if (j > 2) return;
    }
    s = "a";
}

void makeEntry(std::string& word)
{
    clean(word);
    if (!fsys::exists(path + word) && word != "a")
    {
        file.open(path + word);
        file << id << " " << word;
        id++;
        file.close();
        file.clear();
    }
}

std::string listFiles(std::string path, int& n, int& length)
{
    const fsys::path target_path{path};
    std::string list = "";
    n = 0;
    try
    {
        for (auto const& dir_entry : fsys::directory_iterator{target_path})
        {
            if (fsys::is_regular_file(dir_entry.path()))
                list+= dir_entry.path().filename().string();
            n++;
        }
        length = list.length() / n;
    }
    catch (fsys::filesystem_error const& ex)
    {
        std::cout << "Error occurred during file operations!\n" << ex.what() << std::endl;
        list = "";
    }
    return list;
}

std::string fetchData(const char* fname)
{
    char content[65536];
    std::string text = "";
    try
    {
        FILE* file = fopen(fname, "rb");
        
        FileReadStream istream(file, content, sizeof(content));
        Document document;
        
        document.ParseStream(istream);
        fclose(file);

        text += document["metadata"]["title"].GetString();
        text += " ";
        for (const auto& paragraph : document["abstract"].GetArray())
        {
            text += paragraph["text"].GetString();
            text += " ";
        }
        
        for (const auto& paragraph : document["body_text"].GetArray())
        {
            text += paragraph["text"].GetString();
            text += " ";
        }
    }
    catch (std::exception e)
    {
        std::cout << "There was an error in the formatting of the file " << fname << "\n";
        text = "";
    }
    return text;
}

void addLexicon(std::string& content)
{
    std::stringstream ss;
    std::string word = "";
    
    fsys::create_directory(path);
    ss << content;
    while (ss >> word)
        makeEntry(word);
    file.open(idfile);
    file << id;
}

int main()
{
    int n = 0, length = 0, i = 0;
    std::string path = ".\\sample\\", list = listFiles(path, n, length), content = "";
    std::string *file_list = new std::string[n];
    
    for (i = 0; i < n; i++)
        file_list[i] = list.substr(i*length, length);

    if (!fsys::exists(idfile))
    {
        file.open(idfile);
        file << id;
    }
    infile.open(idfile);
    infile >> id;
    for (i = 0; i < n; i++)
    {
        content = fetchData((path + file_list[i]).c_str());
        addLexicon(content);
    }
    return 0;
}