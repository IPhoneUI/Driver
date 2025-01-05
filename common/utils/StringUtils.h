#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>
#include <vector>
#include <limits.h>
#include <chrono>

namespace utils {

class StringUtils
{
public:
    /**
     * @brief split : split the input string with the delim.
     * @param str   : the string to be split
     * @param delim : pattern of the cutting cursor
     * @return vector array of splitted strings.
     */
    static std::vector<std::string> split(const std::string& str, const std::string& delim)
    {
        std::vector<std::string> temp;
        auto start = 0U;
        auto end = str.find(delim);
        while (end != std::string::npos)
        {
            temp.push_back( str.substr(start, end - start));
            start = end + delim.length();
            end = str.find(delim, start);
        }
        if(start < str.size()) 
        {
            temp.push_back( str.substr(start, str.size() - start));
        }
        return temp;
    }
private:
    StringUtils();
};

}

#endif // STRINGUTILS_H
