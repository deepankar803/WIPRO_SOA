#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <cstring>
#include <fstream>

void listFiles(const char *path)
{
    DIR *dir = opendir(path);
    if (dir)
    {
        struct dirent *entry;
        while ((entry = readdir(dir)))
        {
            std::cout << entry->d_name << std::endl;
        }
        closedir(dir);
    }
}

void copyFile(const char *src, const char *dst)
{
    std::ifstream in(src, std::ios::binary);
    std::ofstream out(dst, std::ios::binary);
    out << in.rdbuf();
}

int main()
{
    char currentPath[1024] = ".";
    std::string command;
    while (true)
    {
        std::cout << "Current Directory: " << currentPath << std::endl;
        listFiles(currentPath);
        std::cout << "Command (cd <dir>, copy <src> <dst>, move <src> <dst>, delete <file>, create <file>, or exit): ";
        std::getline(std::cin, command);
        if (command == "exit")
            break;
        if (command.substr(0, 3) == "cd ")
        {
            std::string newDir = command.substr(3);
            if (chdir(newDir.c_str()) == 0)
            {
                getcwd(currentPath, sizeof(currentPath));
            }
        }
        else if (command.substr(0, 5) == "copy ")
        {
            size_t pos = command.find(' ', 5);
            std::string src = command.substr(5, pos - 5);
            std::string dst = command.substr(pos + 1);
            copyFile(src.c_str(), dst.c_str());
        }
        else if (command.substr(0, 5) == "move ")
        {
            size_t pos = command.find(' ', 5);
            std::string src = command.substr(5, pos - 5);
            std::string dst = command.substr(pos + 1);
            rename(src.c_str(), dst.c_str());
        }
        else if (command.substr(0, 7) == "delete ")
        {
            std::string file = command.substr(7);
            remove(file.c_str());
        }
        else if (command.substr(0, 7) == "create ")
        {
            std::string file = command.substr(7);
            std::ofstream out(file);
        }
    }
    return 0;
}