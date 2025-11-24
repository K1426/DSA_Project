#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <iostream>
 
using namespace rapidjson;

int main()
{
    FILE* file = fopen("myfile.json", "rb");
    char content[65536];
    
    FileReadStream istream(file, content, sizeof(content));
    Document document;
    
    document.ParseStream(istream);
    fclose(file);

    std::cout << document["paper_id"].GetString() << std::endl << std::endl;
    std::cout <<document["abstract"][0]["text"].GetString() << std::endl << std::endl;
    
    for (const auto& paragraph : document["body_text"].GetArray())
        std::cout << paragraph["text"].GetString() << std::endl << std::endl;
    return 0;
}