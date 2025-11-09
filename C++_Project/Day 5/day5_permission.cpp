#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <stack>
#include <sys/stat.h>

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

void searchFiles(const char *root, const char *name)
{
    std::stack<std::string> dirs;
    dirs.push(root);
    while (!dirs.empty())
    {
        std::string current = dirs.top();
        dirs.pop();
        DIR *dir = opendir(current.c_str());
        if (dir)
        {
            struct dirent *entry;
            while ((entry = readdir(dir)))
            {
                if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                    continue;
                std::string fullPath = current + "/" + entry->d_name;
                if (strcmp(entry->d_name, name) == 0)
                {
                    std::cout << "Found: " << fullPath << std::endl;
                }
                if (entry->d_type == DT_DIR)
                {
                    dirs.push(fullPath);
                }
            }
            closedir(dir);
        }
    }
}

int main()
{
    char currentPath[1024] = ".";
    std::string command;
    while (true)
    {
        std::cout << "Current Directory: " << currentPath << std::endl;
        listFiles(currentPath);
        std::cout << "Command (cd <dir>, copy <src> <dst>, move <src> <dst>, delete <file>, create <file>, search <name>, permissions <file>, chmod <mode> <file>, or exit): ";
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
        else if (command.substr(0, 7) == "search ")
        {
            std::string name = command.substr(7);
            searchFiles(currentPath, name.c_str());
        }
        else if (command.substr(0, 12) == "permissions ")
        {
            std::string file = command.substr(12);
            struct stat st;
            if (stat(file.c_str(), &st) == 0)
            {
                std::cout << "Permissions: " << std::oct << (st.st_mode & 0777) << std::dec << std::endl;
            }
        }
        else if (command.substr(0, 6) == "chmod ")
        {
            size_t pos = command.find(' ', 6);
            std::string modeStr = command.substr(6, pos - 6);
            std::string file = command.substr(pos + 1);
            int mode = strtol(modeStr.c_str(), nullptr, 8);
            chmod(file.c_str(), mode);
        }
    }
    return 0;
}