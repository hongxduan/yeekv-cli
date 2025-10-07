//
// Created by HongXing on 1/10/25.
//

#include "response.h"

#include <iostream>
#include <sstream>

#include "kvtp.h"
#include "../util/byte_util.h"

void kvtp::decode_response(std::vector<uint8_t> raw_res) {
    std::string raw_str = std::string(raw_res.begin(), raw_res.end());
    std::stringstream stream(raw_str);
    std::string tmp;
    size_t line_num = 0;
    size_t header_size = 0;

    std::string protocol_status;
    std::string data_type;
    //
    // decode header
    //
    while (std::getline(stream, tmp, LINE_FEED)) {
        // accumulate header size
        header_size += tmp.size();
        header_size += 1; // ending '\0'

        if (line_num == 0) {
            protocol_status = tmp;
        } else if (line_num == 1) {
            data_type = tmp;
        } else {
            // if meet empty line, i.e. header and body seperator
            // then end the while loop
            if (tmp == "") {
                break;
            }
        }
        line_num++;
    }

    // split protocol and status
    // KVTP/V1 OK|ERR
    std::string status;
    std::stringstream ss(protocol_status);
    line_num = 0;
    while (std::getline(ss, tmp, SPACE_CHAR)) {
        if (line_num == 1) {
            status = tmp;
        }
        line_num++;
    }

    //
    // decode body
    //
    std::vector<uint8_t> body_bytes;
    body_bytes.assign(raw_res.begin() + header_size, raw_res.end());

    if (data_type == RES_DT_I) {
        if (body_bytes.size() != 4) {
            // error
        }
        uint8_t bytes[4];
        std::copy(body_bytes.begin(), body_bytes.end(), bytes);
        auto result = util::bytes_to_int32(bytes);
        std::cout << status << ":" << result << std::endl;
    } else if (data_type == RES_DT_L) {
    } else if (data_type == RES_DT_D) {
    } else if (data_type == RES_DT_S) {
        std::string result;
        result.assign(body_bytes.begin(), body_bytes.end());
        std::cout << status << ":" << result << std::endl;
    } else if (data_type == RES_DT_LI) {
    } else if (data_type == RES_DT_LL) {
    } else if (data_type == RES_DT_LD) {
    } else if (data_type == RES_DT_LS) {
    } else if (data_type == RES_DT_M) {
    } else {
        // error type
    }
}
