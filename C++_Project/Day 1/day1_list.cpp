#include <iostream>
#include <dirent.h>
#include <cstring>
#include <unistd.h>
void listFiles(const std::string &path)
{
    DIR *dir = opendir(path.c_str());
    if (dir == nullptr)
    {
        std::cerr << "Error opening directory: " << path << std::endl;
        return;
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr)
    {
        std::cout << entry->d_name << std::endl;
    }
    closedir(dir);
}
int main()
{
    std::string currentPath = ".";
    std::cout << "File Explorer - Current Directory: " << currentPath << std::endl;
    listFiles(currentPath);
    return 0;
}
