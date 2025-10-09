//
// Created by HongXing on 1/10/25.
//

#include "string_util.h"

#include <string>
#include <algorithm>

std::string util::to_upper(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

std::string util::trim_left(const std::string &str) {
    size_t start = str.find_first_not_of(" \t\n");
    return (start == std::string::npos) ? "" : str.substr(start);
}

std::string util::trim_right(const std::string &str) {
    size_t end = str.find_last_not_of(" \t\n");
    return (end == std::string::npos) ? "" : str.substr(0, end + 1);
}

std::string util::trim(const std::string &str) {
    return trim_right(trim_left(str));
}