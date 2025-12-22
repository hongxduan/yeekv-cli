//
// Created by HongXing on 1/10/25.
//

#ifndef YEEKV_CLI_STRING_UTILITY_H
#define YEEKV_CLI_STRING_UTILITY_H
#include <string>

namespace util {
    std::string to_upper(std::string str);

    std::string trim_left(const std::string &str);

    std::string trim_right(const std::string &str);

    std::string trim(const std::string &str);
}




#endif //YEEKV_CLI_STRING_UTILITY_H