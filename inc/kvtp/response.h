// Author: Duan Hongxing
// Copyright(c) 2025 Duan Hongxing, hongxduan@gmail.com
//
// Updates:
//  2025/10/01: Create
//

#ifndef YEEKV_CLI_KVTPRESPONSE_H
#define YEEKV_CLI_KVTPRESPONSE_H
#include <cinttypes>
#include <string>
#include <vector>

// response data type define
#define RES_DT_I "I"    // Integer 32 bit
#define RES_DT_L "L"    // Long 64 bit
#define RES_DT_D "D"    // Double
#define RES_DT_S "S"    // String
#define RES_DT_H "H"    // Hash
#define RES_DT_LI "LI"  // List of Integer
#define RES_DT_LL "LL"  // List of Long
#define RES_DT_LD "LD"  // List of Double
#define RES_DT_LS "LS"  // List of String

namespace kvtp {

const std::string STATUS_OK = "OK";
const std::string STATUS_ERR = "ERR";

/// Decode kvtp response from server
/// @param raw_res
/// @return
void decode_response(std::vector<uint8_t> raw_res);
}  // namespace kvtp

#endif  // YEEKV_CLI_KVTPRESPONSE_H
