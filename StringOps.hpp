#ifndef STRING_OPS_HPP
#define STRING_OPS_HPP

#include <cstdint>
#include <string>
#include <vector>


template<typename Out>
void split(const std::string &s, char delim, Out result);

std::vector<std::string> split(const std::string &s, char delim);

template <typename T>
std::string ts(const T& t);

template <>
std::string ts(const bool& t);

template <>
std::string ts(const uint32_t& t);

template <>
std::string ts(const int32_t& t);

template <>
std::string ts(const uint64_t& t);

template <>
std::string ts(const int64_t& t);


#endif
