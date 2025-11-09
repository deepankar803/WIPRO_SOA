#include <iostream>
#include <dirent.h>
#include <cstring>
#include <unistd.h>
#include <limits>
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
    std::string command;
    while (true)
    {
        std::cout << "File Explorer - Current Directory: " << currentPath << std::endl;
        listFiles(currentPath);
        std::cout << "Enter command (cd <dir> or exit): ";
        std::getline(std::cin, command);
        if (command == "exit")
            break;
        if (command.substr(0, 3) == "cd ")
        {
            std::string newDir = command.substr(3);
            if (chdir(newDir.c_str()) == 0)
            {
                char cwd[1024];
                getcwd(cwd, sizeof(cwd));
                currentPath = cwd;
            }
            else
            {
                std::cerr << "Error changing directory." << std::endl;
            }
        }
    }
    return 0;
}
