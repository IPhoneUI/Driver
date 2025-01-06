#include "filesystem.h"

namespace common::filesystem
{
    bool exists(const std::string &path)
    {
        boost::system::error_code ec;
        const bool res = boost::filesystem::exists(path, ec);
        if (ec)
        {
            LOG_WARN("path: %s is not a file", path.c_str());
            return false;
        }
        return res;
    }

    bool is_directory(const std::string &path)
    {
        boost::system::error_code ec;
        const bool res = boost::filesystem::is_directory(path, ec);
        if (ec)
        {
            LOG_WARN("path: %s is not directory", path.c_str());
            return false;
        }
        return res;
    }

    void remove_folder_contents(const std::string &path)
    {
        for (boost::filesystem::directory_iterator end_dir_it, it(path); it != end_dir_it; ++it)
        {
            boost::filesystem::remove_all(it->path());
        }
    }

    void remove(const std::string &path)
    {
        boost::system::error_code ec;
        boost::filesystem::remove(path, ec);
        if (ec)
        {
            LOG_ERR("remove given file value: %d, message: %s, path: %s", ec.value(), ec.message().c_str(), path.c_str());
        }
    }

    std::string filename(const std::string &path)
    {
        return boost::filesystem::path(path).filename().generic_string();
    }

    std::string parent_path(const std::string &path)
    {
        return boost::filesystem::path(path).parent_path().generic_string();
    }

    bool read(const std::string &path, std::string &res)
    {
        try
        {
            std::ifstream t(path);
            if (t && !t.fail())
            {
                t.seekg(0, t.end);
                const size_t size = t.tellg();
                if (size != (size_t)-1)
                {
                    res.reserve(size);
                }
                t.seekg(0, t.beg);
                res.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
                return !t.fail();
            }
            return false;
        }
        catch (...)
        {
            LOG_ERR("Unexpected exception during file read path: %s, info: %s", path, boost::current_exception_diagnostic_information());
            return false;
        }
    }

    boost::property_tree::ptree read_json(const std::string &filename)
    {
        boost::property_tree::ptree ptree;
        try 
        {
            boost::property_tree::read_json(filename, ptree);
        } catch (const boost::property_tree::json_parser_error &e) {
            LOG_ERR("Error reading JSON file: %s", e.what());
        }

        return ptree;
    }

    unsigned int read_file_line_count(const std::string &path)
    {
        std::ifstream fileHandler(path);
        if (!fileHandler)
        {
            return 0;
        }
        std::string line;
        unsigned int count = 0;
        while (std::getline(fileHandler, line))
        {
            ++count;
        }
        return count;
    }

    unsigned int read_file_size(const std::string &path)
    {
        std::ifstream in_file(path, std::ios::binary);
        if (!in_file)
        {
            LOG_ERR("Repository Read Error %s", path);
            return 0;
        }
        in_file.seekg(0, std::ios::end);
        unsigned int file_size = in_file.tellg();
        in_file.close();

        return file_size;
    }

    bool write(const std::string &path, const std::string &content, const bool sync)
    {
        const auto fd = ::open(path.c_str(), sync ? O_CREAT | O_WRONLY | O_DSYNC | O_TRUNC : O_CREAT | O_WRONLY | O_TRUNC, 0666);
        if (fd)
        {
            ::write(fd, content.data(), content.size());
            if (sync)
            {
                ::fsync(fd);
            }
            ::close(fd);
        }
        else
        {
            LOG_ERR("Failed to open file %s", path);
        }
        return true;
    }

    bool append(const std::string &path, const std::string &content, const bool sync)
    {
        const auto fd = ::open(path.c_str(), sync ? O_CREAT | O_WRONLY | O_DSYNC | O_APPEND : O_CREAT | O_WRONLY | O_APPEND, 0666);
        if (fd)
        {
            ::write(fd, content.data(), content.size());
            if (sync)
            {
                ::fsync(fd);
            }
            ::close(fd);
        }
        else
        {
            LOG_ERR("Failed to open file %s", path);
        }
        return true;
    }

    bool create_file(const std::string &path)
    {
        std::ofstream{path};
        return true;
    }

    bool create_dir(const std::string &path)
    {
        if (is_directory(path))
        {
            return true;
        }
        boost::system::error_code ec;
        const auto ret = boost::filesystem::create_directories(path, ec);
        if (ec)
        {
            LOG_ERR("create directory value: %d, message: %s, path: %s", ec.value(), ec.message().c_str(), path.c_str());
        }
        return ret;
    }

    bool rename(const std::string &old, const std::string &name)
    {
        boost::system::error_code ec;
        boost::filesystem::rename(old, name, ec);
        if (ec)
        {
            LOG_ERR("remove the old : %s, name: %s", old, name);
            return false;
        }
        return true;
    }

    void read_dir(const std::string &path, std::function<void(const std::string &)> callback)
    {
        if (access(path.c_str(), F_OK) == 0)
        {
            auto *dirp = opendir(path.c_str());
            if (dirp != nullptr)
            {
                struct dirent *entry = nullptr;
                while ((entry = readdir(dirp)) != nullptr)
                {
                    const std::string filename = entry->d_name;
                    if (filename != "." && filename != "..")
                    {
                        callback(filename);
                    }
                }
                closedir(dirp);
            }
        }
    }

    std::vector<std::string> read_dir(const std::string &path)
    {
        std::vector<std::string> files;
        read_dir(path, [&](const auto &filename) { 
            files.push_back(filename); 
        });
        return files;
    }
}