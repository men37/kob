#include "StringOps.hpp"

#include <sstream>

template<typename Out>
void split(const std::string &s, char delim, Out result)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

template <typename T>
std::string ts(const T& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

template <>
std::string ts(const bool& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

template <>
std::string ts(const uint32_t& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

template <>
std::string ts(const int32_t& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

template <>
std::string ts(const uint64_t& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

template <>
std::string ts(const int64_t& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}
