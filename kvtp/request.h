//
// Created by HongXing on 1/10/25.
//

#ifndef YEEKV_CLI_KVTPREQUEST_H
#define YEEKV_CLI_KVTPREQUEST_H
#include <vector>

#include "../parser/input_parser.h"


#define ZERO '\0'

#define LINE_FEED '\n'

#define PROTOCOL "KVTP/1"
#define KEY_PREFIX "KEY:"
#define CMD_PREFIX "CMD:"
#define ARG_PREFIX "ARGS:"
#define TTL_PREFIX "TTL:"

namespace kvtp {
    // encode user input command to KVTP request
    std::vector<uint8_t> encodeRequest(InputData data);
}


#endif //YEEKV_CLI_KVTPREQUEST_H
