#ifndef SRTCONV_H
#define SRTCONV_H

#include <string>
#include <cctype>
#include <ostream>
#include <sstream>
#include <iostream>
#include <vector>
#include <locale>
#include <format>

namespace util {

// String comparison functions
inline bool strEqual(const std::string &str1, const std::string &str2) {
    return str1 == str2;
}

inline bool strIequal(const std::string &str1, const std::string &str2) {
    return std::equal(str1.begin(), str1.end(), str2.begin(), str2.end(),
                      [](char c1, char c2) { return std::toupper(c1) == std::toupper(c2); });
}

inline bool strLexicalCompare(const std::string &str1, const std::string &str2) {
    return std::lexicographical_compare(str1.begin(), str1.end(), str2.begin(), str2.end());
}

// String trimming functions
inline std::string ltrim(std::string s, const char* t = " \t\n\r\f\v") {
    s.erase(0, s.find_first_not_of(t));
    return s;
}

inline std::string rtrim(std::string s, const char* t = " \t\n\r\f\v") {
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

inline std::string trim(std::string s, const char* t = " \t\n\r\f\v") {
    return ltrim(rtrim(s, t), t);
}

// String case conversion
inline std::string upper(const std::string &str) {
    std::string newstr = str;
    std::transform(newstr.begin(), newstr.end(), newstr.begin(), ::toupper);
    return newstr;
}

// String splitting
inline std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream tokenStream(s);
    std::string token;
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Float to string conversion with formatting
inline std::string floatToString(double dv, int decimals = 2,
                                 char thousandSeparator = '.',
                                 char decimalSeparator = ',',
                                 const std::string &prefix = "",
                                 const std::string &suffix = "") {
    std::ostringstream ss;
    ss.imbue(std::locale(""));
    ss.precision(decimals);
    ss.setf(std::ios::fixed);
    ss << dv;

    std::string str = ss.str();
    std::replace(str.begin(), str.end(), '.', thousandSeparator);
    str.replace(str.length() - (decimals + 1), 1, std::string(1, decimalSeparator));

    return prefix + str + suffix;
}

// String replacement
inline std::string replace(std::string str, const std::string &old, const std::string &New) {
    size_t pos = 0;
    while ((pos = str.find(old, pos)) != std::string::npos) {
        str.replace(pos, old.size(), New);
        pos += New.size();
    }
    return str;
}

// Conversion functions
template<typename T>
int toInt(T v) {
    return std::stoi(std::to_string(v));
}

template<typename T>
double toDouble(T v) {
    return std::stod(std::to_string(v));
}

template<typename T>
std::string toStr(T v) {
    return replace(std::to_string(v), "\"", "");
}

// Type checking functions
template<typename T>
bool isInt(T v) {
    std::string s = toStr(v);
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

template<typename T>
bool isFloat(T v) {
    std::string s = toStr(v);
    std::istringstream iss(s);
    double d;
    return iss >> d && iss.eof();
}

// String to float conversion
inline double strToFloat(const std::string &v) {
    std::string s = replace(v, ".", "#");
    s = replace(s, ",", ".");
    s = replace(s, "#", ",");
    return std::strtod(s.c_str(), nullptr);
}

// Date conversion functions
inline std::string dateFromSql(const std::string &v, char c = '-') {
    auto data = split(v, c);
    int year = std::stoi(data[0]);
    int month = std::stoi(data[1]);
    int day = std::stoi(data[2]);
    return std::format("{:02}/{:02}/{:04}", day, month, year);
}

inline std::string dateToSql(const std::string &v, char c = '/') {
    auto data = split(v, c);
    int day = std::stoi(data[0]);
    int month = std::stoi(data[1]);
    int year = std::stoi(data[2]);
    return std::format("{:04}-{:02}-{:02}", year, month, day);
}

} // namespace util

// Map utility functions
template<typename T>
bool hasKey(const T &map, const std::string &x) {
    return map.count(x) > 0;
}

template<typename T>
auto mapKey(const T &map, const std::string &x) -> decltype(map.at(x)) {
    try {
        return map.at(x);
    } catch (const std::out_of_range &e) {
        std::cerr << "map key " << x << " not defined\n";
        throw;
    }
}

#endif // SRTCONV_H