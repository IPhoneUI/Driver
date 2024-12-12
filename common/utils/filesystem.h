#include <string>
#include <functional>
#include <vector>
#include "boost/filesystem.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/exception/diagnostic_information.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>
#include <string>
#include <fstream>
#include <streambuf>
#include <iomanip>
#include <ctime>
#include <cstdio>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <utils/Logger.h>
#include <boost/algorithm/string.hpp>

namespace common::filesystem
{

    /**
     * @brief Returns true if a file or directory exists at the given path.
     *
     * @param path
     * @return true
     * @return false
     */
    bool exists(const std::string &path);

    /**
     * @brief Returns true if the path is a directory.
     *
     * @param path
     * @return true
     * @return false
     */
    bool is_directory(const std::string &path);

    /**
     * Remove all contents of folder
     */
    void remove_folder_contents(const std::string &path);

    /**
     * @brief Removes the given file.
     *
     * @param path
     */
    void remove(const std::string &path);

    /**
     * @brief Gets the filename portion of a path.
     *
     * @param path
     */
    std::string filename(const std::string &path);

    /**
     * @brief Gets the parent path of a path
     *
     * @param path
     */
    std::string parent_path(const std::string &path);

    /**
     * @brief Gets the contents of the file into the res string
     *
     * @param path, res
     */
    bool read(const std::string &path, std::string &res);

    /**
     * @brief Read json file
     *
     * @param path, res
     */

    /**
     * @brief Gets the contents of the file
     *
     * @param filename
     */
    boost::property_tree::ptree read_json(const std::string &filename);

    /**
     * @brief Reads the line count of file of a path
     *
     * @param path
     */
    unsigned int read_file_line_count(const std::string &path);

    /**
     * @brief Reads the size of file of a path
     *
     * @param path
     */
    unsigned int read_file_size(const std::string &path);

    /**
     * @brief Write the content into a file of a path. This content will be replaced with the latest
     *
     * @param path, content
     * @param sync: Use O_DSYNC flag. Data will be synchronized immediately after writing, ensuring that the data has been completely written to the disk.
     *              Performance is reduced because you have to wait for data to be written to disk.
     */
    bool write(const std::string &path, const std::string &content, const bool sync);

    /**
     * @brief Append the content into a file of a path. This content will not be replaced with the latest
     *
     * @param path, content
     * @param sync: Use O_DSYNC flag. Data will be synchronized immediately after writing, ensuring that the data has been completely written to the disk.
     *              Performance is reduced because you have to wait for data to be written to disk.
     */
    bool append(const std::string &path, const std::string &content, const bool sync);

    /**
     * @brief Creates the new file of a path
     *
     * @param path
     */
    bool create_file(const std::string &path);

    /**
     * @brief Creates the new folder of a path
     *
     * @param path
     */
    bool create_dir(const std::string &path);

    /**
     * @brief Rename the old file or folder to the new one
     *
     * @param old, name
     */
    bool rename(const std::string &old, const std::string &name);

    /**
     * @brief Read the whole file or folder into a directory
     *
     * @param path, callback
     */
    void read_dir(const std::string &path, std::function<void(const std::string &)> callback);

    /**
     * @brief Read the whole file or folder into a directory
     *
     * @param path
     */
    std::vector<std::string> read_dir(const std::string &path);
}