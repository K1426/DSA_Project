#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <filesystem>
#include <bits/stdc++.h>
using namespace rapidjson;
namespace fsys = std::filesystem;

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
        text += "\n\n";
        for (const auto& paragraph : document["abstract"].GetArray())
        {
            text += paragraph["text"].GetString();
            text += "\n\n";
        }
        
        for (const auto& paragraph : document["body_text"].GetArray())
        {
            text += paragraph["text"].GetString();
            text += "\n\n";
        }
    }
    catch (std::exception e)
    {
        std::cout << "There was an error in the formatting of the file " << fname << "\n";
        text = "";
    }
    return text;
}

int main()
{
    int n = 0, length = 0;
    std::string path = "D:\\cord-19_2020-06-01\\2020-06-01\\sample\\", list = listFiles(path, n, length);
    std::string *file_list = new std::string[n], *content_list = new std::string[n];
    
    for (int i = 0; i < n; i++)
        file_list[i] = list.substr(i*length, length);
    for (int i = 0; i < n; i++)
        content_list[i] = fetchData((path + file_list[i]).c_str());
    
    std::cout << content_list[0];
    return 0;
}