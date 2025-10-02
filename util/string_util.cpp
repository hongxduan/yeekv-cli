//
// Created by HongXing on 1/10/25.
//

#include "string_util.h"

#include <string>
#include <algorithm>

std::string util::toUpper(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}
