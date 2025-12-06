//
// Created by HongXing on 1/10/25.
//

#include "response.h"
#include "kvtp.h"
#include "../util/byte_util.h"

#include <iostream>
#include <map>
#include <sstream>

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

    // body is empty
    if (header_size > raw_res.size()) {
        std::cout << status << ":" << std::endl;
        return;
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
        int read = 0;
        int idx = 0;
        while (read < body_bytes.size()) {
            // read item length
            uint8_t len_bytes[4];
            std::copy(body_bytes.begin() + read, body_bytes.begin() + read + 4, len_bytes);
            read += 4;
            auto len = util::bytes_to_int32(len_bytes);
            if (read + len > body_bytes.size()) {
                std::cout << "ERR:" << "DATA_ERR" << std::endl;
            }
            // read item
            uint8_t bytes[len];
            std::copy(body_bytes.begin() + read, body_bytes.begin() + read + len, bytes);
            read += len;
            auto val = std::string(bytes, bytes + len);
            ++idx;
            // print
            std::cout << idx << ":" << val << std::endl;
        }
    } else if (data_type == RES_DT_H) {
        uint16_t pos = 0;
        std::map<std::string, std::string> result;
        constexpr int field_byte_count = 2;
        constexpr int value_byte_count = 4;
        while (pos < body_bytes.size() - (field_byte_count + value_byte_count)) {
            uint16_t len = 0;
            std::vector<u_char> len_bytes;
            std::vector<u_char> fv_bytes = {}; // bytes for field and value

            // Get field length
            len_bytes.assign(body_bytes.begin() + pos, body_bytes.begin() + pos + field_byte_count);
            std::string s_num(len_bytes.begin(), len_bytes.end());
            len = util::bytes_to_uint16(len_bytes.data());
            pos += field_byte_count; // Forward pos

            if (pos + len >= body_bytes.size()) {
                std::cout << "ERR:" << "DATA_ERR" << std::endl;
                return;
            }

            // Get filed
            fv_bytes.assign(body_bytes.begin() + pos, body_bytes.begin() + pos + len);
            std::string field;
            field.assign(fv_bytes.begin(), fv_bytes.end());
            //fields.push_back(field);
            pos += len; // Forward pos

            len_bytes.clear();
            fv_bytes.clear();

            // Get value length
            len_bytes.assign(body_bytes.begin() + pos, body_bytes.begin() + pos + value_byte_count);
            len = util::bytes_to_uint32(len_bytes.data());
            pos += value_byte_count; // Forward pos
            if (pos + len > body_bytes.size()) {
                std::cout << "ERR:" << "DATA_ERR" << std::endl;
                return;
            }

            // Get value
            fv_bytes.assign(body_bytes.begin() + pos, body_bytes.begin() + pos + len);
            std::string val;
            val.assign(fv_bytes.begin(), fv_bytes.end());
            //values.push_back(value);
            pos += len; // Forward pos

            result.insert(std::make_pair(field, val));
        }
        for (auto &[f,v]: result) {
            std::cout << f << ":" << v << std::endl;
        }
    } else {
        // error type
    }
}
