//
// Created by HongXing on 1/10/25.
//
// Parse user command line input
//

#ifndef YEEKV_CLI_INPUT_PARSE_H
#define YEEKV_CLI_INPUT_PARSE_H


#define SPACE_CHAR  ' '
#define DQUOTE_CHAR  '"'
#define BSLASH_CHAR  '\\'

#define ARG_EX  "-ex"
#define ARG_NX  "-nx"
#define ARG_DEL  "-d"
#define ARG_TTL  "-ttl"

#include <string>
#include <vector>

struct InputData {
    std::string error; // if has error
    std::string cmd; // the command
    std::string key; // the key
    std::vector<std::string> args; // arguments
    uint32_t ttl; // ttl
    std::string value; // the value
};

InputData parseInput(std::string input);


#endif //YEEKV_CLI_INPUT_PARSE_H
