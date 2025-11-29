//
// Created by HongXing on 1/10/25.
//

#ifndef YEEKV_CLI_KVTPRESPONSE_H
#define YEEKV_CLI_KVTPRESPONSE_H
#include <cinttypes>
#include <vector>

// response data type define
#define RES_DT_I "I"    // Integer 32 bit
#define RES_DT_L "L"    // Long 64 bit
#define RES_DT_D "D"    // Double
#define RES_DT_S "S"    // String
#define RES_DT_LI "LI"  // List of Integer
#define RES_DT_LL "LL"  // List of Long
#define RES_DT_LD "LD"  // List of Double
#define RES_DT_LS "LS"  // List of String
#define RES_DT_M  "M"   // Map

namespace kvtp {
    // decode KVTP response from server
    void decode_response(std::vector<uint8_t>);
}


#endif //YEEKV_CLI_KVTPRESPONSE_H
