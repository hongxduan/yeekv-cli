//
// Created by HongXing on 1/10/25.
//

#ifndef YEEKV_CLI_KVTPREQUEST_H
#define YEEKV_CLI_KVTPREQUEST_H
#include <vector>



namespace kvtp {
    std::vector<uint8_t> encodeRequest(char *line);
}


#endif //YEEKV_CLI_KVTPREQUEST_H
