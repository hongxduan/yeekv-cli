//
// Created by HongXing on 1/10/25.
//

#ifndef YEEKV_CLI_KVTPRESPONSE_H
#define YEEKV_CLI_KVTPRESPONSE_H
#include <vector>


namespace kvtp {
    // decode KVTP response from server
    std::vector<uint8_t> decodeResponse(std::vector<uint8_t>);
}


#endif //YEEKV_CLI_KVTPRESPONSE_H
