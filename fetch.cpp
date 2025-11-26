#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <filesystem>
#include <bits/stdc++.h>
using namespace rapidjson;
namespace fsys = std::filesystem;

void clean(std::string& s)
{
    s.erase(std::remove_if(s.begin(), s.end(), [](char c) { return !(isalnum(c) || c == '-' || c == '@'); }), s.end());
    if (s.length() < 3 || std::all_of(s.begin(), s.end(), {return !isalpha(c);})) s = "a";
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

void addLexicon(std::string content)
{
    std::stringstream ss;
    std::ofstream file;
    std::string path = "D:\\DSA_Project\\DSA_Project\\lexicon\\", word = "";
    int i = 0;
    
    fsys::create_directory(path);
    ss << content;
    while (ss >> word)
    {
        clean(word);
        if (!fsys::exists(path + word) && word != "a")
        {
            file.open(path + word);
            file << i << " " << word;
            i++;
            file.close();
            file.clear();
        }
    }

}

int main()
{
    int n = 0, length = 0, i = 0;
    std::string path = "D:\\DSA_Project\\DSA_Project\\sample\\", list = listFiles(path, n, length), content = "";
    std::string *file_list = new std::string[n];
    
    
    for (i = 0; i < n; i++)
        file_list[i] = list.substr(i*length, length);
    for (i = 0; i < n; i++)
    {
        content = fetchData((path + file_list[i]).c_str());
        addLexicon(content);
    }
    return 0;
}