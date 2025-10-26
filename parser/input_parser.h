//
// Created by HongXing on 1/10/25.
//
// Parse user command line input
//

#ifndef YEEKV_CLI_INPUT_PARSE_H
#define YEEKV_CLI_INPUT_PARSE_H


#define SPACE_CHAR      ' '
#define DQUOTE_CHAR     '"'
#define BSLASH_CHAR     '\\'

#define ARG_EX  "-ex"
#define ARG_NX  "-nx"
#define ARG_DEL "-del"
#define ARG_TTL "-ttl"
#define ARG_INC "-inc"

// For shard use
#define ARG_ID "-id"

#define GET "GET"
#define SET "SET"
#define DEL "DEL"
#define KEY "KEY"
#define SHARD "SHARD"
#define RESHARD "RESHARD"

#include <string>
#include <vector>

struct InputData {
    std::string error; // if it has error
    std::string cmd; // the command
    std::string key; // the key
    std::vector<std::string> args; // arguments
    //uint32_t ttl; // ttl
    std::string ttl;
    std::string inc;
    std::string id; // for shard command
    std::string value; // the value
};

/// Parse user input to InputData
/// @param input
/// @return
InputData parse_input(std::string input);

/// Parse command that has KEY
void parse_key_input(std::vector<std::string> pieces, InputData &data);

/// Parse command that has NO KEY
/// The KEY head may use to store sub command
/// @param pieces
/// @param data
void parse_nonkey_input(std::vector<std::string> pieces, InputData &data);

bool is_key_command(std::string cmd);


#endif //YEEKV_CLI_INPUT_PARSE_H
