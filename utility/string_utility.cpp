//
// Created by HongXing on 1/10/25.
//

#include "string_utility.h"

#include <string>

std::string utility::toUpper(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}
